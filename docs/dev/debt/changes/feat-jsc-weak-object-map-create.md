# libSceJsc

## add Functional

- [JSWeakObjectMapCreate and JSWeakObjectMapClear](../../../core/libs/prx/libSceJsc/src/ApiBridge.cpp) (libSceJsc) - not compared with a console module. Create forwards to libSceNKWebKit like the bridge entry above. Both follow pinned [JSWeakObjectMapRefPrivate.cpp](https://github.com/WebKit/WebKit/blob/73a35b43be7ae44c084885bd3fa4b403e59723c8/Source/JavaScriptCore/API/JSWeakObjectMapRefPrivate.cpp#L40-L95). The map belongs to the context, as WebKit registers it with the global object, and the destroyed callback runs once with the map and private data when the context is destroyed, where WebKit runs it when the collector destroys the global object. A null context or callback throws where WebKit faults. Clear ignores its arguments and returns true, as in WebKit. Closes when: compared with a console module.

## remove | `JSWeakObjectMapClear` | `x94EvR+FAZc` |

## change The 130 throwing JavaScriptCore, Inspector, WTF and bmalloc exports

- The 128 throwing JavaScriptCore, Inspector, WTF and bmalloc exports in [Export.cpp](../../../core/libs/prx/libSceJsc/Export.cpp) are present in the PS5 3.20 `libSceJsc` export inventory from [PS5-3.20_Libs](https://github.com/DNNDHH/PS5-3.20_Libs). Each is named after the symbol the [verified NID name database](https://github.com/zecoxao/sce_symbols/blob/2883963a0a514ba6e77407a08ef4730a65e88254/aerolib.csv) gives for its NID, and the NID of that name matches. Prototypes and behavior are unverified; each uses the no-parameter import-resolution convention and throws through `NotImplemented_nid_no_patch`. These declarations make no ABI or behavior claim.
