#include "prx/libSceNKWebKit/libSceNKWebKit.hpp"
#include "prx/libSceJsc/libSceJsc.hpp"
#include "prx/libc/include/general/ExportMacros.hpp"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

using namespace NKWebKit;

namespace {

void Check(bool value, int line) {
    if (!value) {
        std::fprintf(stderr, "JscApiBridge check failed at line %d\n", line);
        std::abort();
    }
}
#define Require(value) Check((value), __LINE__)

struct ScopedJsString {
    JSStringRef ref = nullptr;
    explicit ScopedJsString(const char* utf8) : ref(JSStringCreateWithUTF8CString_nid_postfix(utf8)) {}
    explicit ScopedJsString(JSStringRef r) : ref(r) {}
    ~ScopedJsString() {
        if (ref) JSStringRelease_nid_postfix(ref);
    }
    ScopedJsString(const ScopedJsString&) = delete;
    ScopedJsString& operator=(const ScopedJsString&) = delete;
    operator JSStringRef() const { return ref; }
};

JSValueRef Eval(JSContextRef ctx, const char* code) {
    ScopedJsString script(code);
    JSValueRef exception = nullptr;
    JSValueRef result = JSEvaluateScript(ctx, script, nullptr, nullptr, 1, &exception);
    Require(exception == nullptr);
    return result;
}

JSValueRef MakeString(JSContextRef ctx, JSStringRef string) {
    return static_cast<JSValueRef>(JSValueMakeString(ctx, string));
}

bool HasPropertyForKey(JSContextRef ctx, JSObjectRef object, JSValueRef key, JSValueRef* exception) {
    return JSObjectHasPropertyForKey(ctx, object, key, reinterpret_cast<const void**>(exception));
}

bool StringValueEquals(JSContextRef ctx, JSValueRef value, const char* expected) {
    if (!JSValueIsString(ctx, value)) return false;
    ScopedJsString copy(JSValueToStringCopy(ctx, value, nullptr));
    return copy.ref && JSStringIsEqualToUTF8CString(copy, expected);
}

bool GlobalCheck(JSContextRef ctx, const char* name, JSValueRef value, const char* predicate) {
    ScopedJsString property(name);
    JSObjectSetProperty(ctx, JSContextGetGlobalObject(ctx), property, value, kJSPropertyAttributeNone, nullptr);
    return JSValueToBoolean(ctx, Eval(ctx, predicate));
}

void TestMakeString(JSGlobalContextRef ctx) {
    ScopedJsString text("h\xC3\xA9llo");
    JSValueRef value = MakeString(ctx, text);
    Require(value != nullptr);
    Require(StringValueEquals(ctx, value, "h\xC3\xA9llo"));
    Require(GlobalCheck(ctx, "made", value, "typeof made === 'string' && made.length === 5 && made.charCodeAt(1) === 0xe9"));

    ScopedJsString empty("");
    JSValueRef emptyValue = MakeString(ctx, empty);
    Require(StringValueEquals(ctx, emptyValue, ""));

    JSValueRef nullValue = MakeString(ctx, nullptr);
    Require(nullValue != nullptr);
    Require(StringValueEquals(ctx, nullValue, ""));
    Require(GlobalCheck(ctx, "fromNull", nullValue, "fromNull === ''"));

    JSValueRef again = MakeString(ctx, text);
    Require(JSValueIsStrictEqual(ctx, value, again));

    Require(JSValueMakeString(nullptr, text) == nullptr);
}

void TestHasPropertyForKey(JSGlobalContextRef ctx) {
    JSObjectRef object = JSValueToObject(ctx, Eval(ctx, "globalThis.sym = Symbol('k'); ({ a: 1, 3: 'x', [sym]: 2 })"), nullptr);
    Require(object != nullptr);

    ScopedJsString a("a");
    ScopedJsString b("b");
    ScopedJsString inherited("toString");
    Require(HasPropertyForKey(ctx, object, MakeString(ctx, a), nullptr));
    Require(!HasPropertyForKey(ctx, object, MakeString(ctx, b), nullptr));
    Require(HasPropertyForKey(ctx, object, MakeString(ctx, inherited), nullptr));
    Require(HasPropertyForKey(ctx, object, JSValueMakeNumber(ctx, 3), nullptr));
    Require(!HasPropertyForKey(ctx, object, JSValueMakeNumber(ctx, 4), nullptr));
    Require(HasPropertyForKey(ctx, object, Eval(ctx, "sym"), nullptr));
    Require(!HasPropertyForKey(ctx, object, Eval(ctx, "Symbol('k')"), nullptr));

    JSObjectRef proxy = JSValueToObject(ctx, Eval(ctx, "new Proxy({}, { has(target, key) { return key === 'trap'; } })"), nullptr);
    ScopedJsString trap("trap");
    Require(HasPropertyForKey(ctx, proxy, MakeString(ctx, trap), nullptr));
    Require(!HasPropertyForKey(ctx, proxy, MakeString(ctx, a), nullptr));

    JSValueRef throwingKey = Eval(ctx, "({ toString() { throw new Error('boom'); } })");
    JSValueRef exception = nullptr;
    Require(!HasPropertyForKey(ctx, object, throwingKey, &exception));
    Require(exception != nullptr);
    Require(GlobalCheck(ctx, "caught", exception, "caught instanceof Error && caught.message === 'boom'"));
    Require(!HasPropertyForKey(ctx, object, throwingKey, nullptr));

    JSObjectRef throwingProxy = JSValueToObject(ctx, Eval(ctx, "new Proxy({}, { has() { throw new TypeError('trap'); } })"), nullptr);
    JSValueRef trapException = nullptr;
    Require(!HasPropertyForKey(ctx, throwingProxy, MakeString(ctx, a), &trapException));
    Require(trapException != nullptr);
    Require(GlobalCheck(ctx, "trapCaught", trapException, "trapCaught instanceof TypeError"));

    exception = nullptr;
    Require(HasPropertyForKey(ctx, object, MakeString(ctx, a), &exception));
    Require(exception == nullptr);

    Require(!JSObjectHasPropertyForKey(nullptr, object, MakeString(ctx, a), nullptr));
    Require(!HasPropertyForKey(ctx, nullptr, MakeString(ctx, a), nullptr));
    Require(!HasPropertyForKey(ctx, object, nullptr, nullptr));
}

struct MapDestroyedState {
    void* map = nullptr;
    void* data = nullptr;
    int calls = 0;
};

void APS5_VABI MapDestroyed(void* map, void* data) {
    auto* state = static_cast<MapDestroyedState*>(data);
    state->map = map;
    state->data = data;
    ++state->calls;
}

JSWeakObjectMapRef AsMap(void* map) {
    return static_cast<JSWeakObjectMapRef>(map);
}

void TestWeakObjectMap() {
    JSGlobalContextRef ctx = JSGlobalContextCreate(nullptr);
    Require(ctx != nullptr);

    MapDestroyedState state;
    void* map = JSWeakObjectMapCreate(ctx, &state, &MapDestroyed);
    Require(map != nullptr);
    MapDestroyedState otherState;
    void* other = JSWeakObjectMapCreate(ctx, &otherState, &MapDestroyed);
    Require(other != nullptr && other != map);

    JSObjectRef object = JSObjectMake(ctx, nullptr, nullptr);
    JSValueProtect(ctx, object);
    int key = 0;
    int otherKey = 0;
    JSWeakObjectMapSet(ctx, AsMap(map), &key, object);
    Require(JSWeakObjectMapGet(ctx, AsMap(map), &key) == object);
    Require(JSWeakObjectMapGet(ctx, AsMap(map), &otherKey) == nullptr);
    Require(JSWeakObjectMapGet(ctx, AsMap(other), &key) == nullptr);

    Require(JSWeakObjectMapClear(ctx, map, &key, object));
    Require(JSWeakObjectMapGet(ctx, AsMap(map), &key) == object);
    Require(JSWeakObjectMapClear(nullptr, nullptr, nullptr, nullptr));

    JSValueUnprotect(ctx, object);
    JSSynchronousGarbageCollectForDebugging(ctx);
    Require(JSWeakObjectMapGet(ctx, AsMap(map), &key) == nullptr);

    bool threw = false;
    try {
        JSWeakObjectMapCreate(nullptr, &state, &MapDestroyed);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw);
    threw = false;
    try {
        JSWeakObjectMapCreate(ctx, &state, nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw);

    Require(state.calls == 0 && otherState.calls == 0);
    JSGlobalContextRelease(ctx);
    Require(state.calls == 1 && state.map == map && state.data == &state);
    Require(otherState.calls == 1 && otherState.map == other && otherState.data == &otherState);
}

void TestWeakObjectMapPerContext() {
    JSGlobalContextRef ctx = JSGlobalContextCreate(nullptr);
    Require(ctx != nullptr);
    JSGlobalContextRef shortLived = JSGlobalContextCreateInGroup(JSContextGetGroup(ctx), nullptr);
    Require(shortLived != nullptr);
    MapDestroyedState kept;
    MapDestroyedState released;
    void* keptMap = JSWeakObjectMapCreate(ctx, &kept, &MapDestroyed);
    void* releasedMap = JSWeakObjectMapCreate(shortLived, &released, &MapDestroyed);
    Require(keptMap != nullptr && releasedMap != nullptr);
    JSGlobalContextRelease(shortLived);
    Require(released.calls == 1 && released.map == releasedMap && released.data == &released);
    Require(kept.calls == 0);

    JSObjectRef object = JSObjectMake(ctx, nullptr, nullptr);
    JSValueProtect(ctx, object);
    int key = 0;
    JSWeakObjectMapSet(ctx, AsMap(keptMap), &key, object);
    Require(JSWeakObjectMapGet(ctx, AsMap(keptMap), &key) == object);
    JSGarbageCollect(ctx);
    Require(JSWeakObjectMapGet(ctx, AsMap(keptMap), &key) == object);
    JSValueUnprotect(ctx, object);

    JSGlobalContextRelease(ctx);
    Require(kept.calls == 1 && kept.map == keptMap && kept.data == &kept);
    Require(released.calls == 1);
}

}

int main() {
    JSGlobalContextRef ctx = JSGlobalContextCreate(nullptr);
    Require(ctx != nullptr);
    TestMakeString(ctx);
    TestHasPropertyForKey(ctx);
    TestWeakObjectMap();
    TestWeakObjectMapPerContext();
    JSGlobalContextRelease(ctx);
    return 0;
}
