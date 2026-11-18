# libSceLibcInternal

## remove | `_ZNSt15basic_streambufIcSt11char_traitsIcEE6xsputnEPKci` | `sXaxl1QGorg` |

## remove | `_ZNSt15basic_streambufIwSt11char_traitsIwEE6xsputnEPKwi` | `041c37QfoUc` |

## add Unknown function info

- [basic_streambuf xsputn](../../../core/libs/prx/libSceLibcInternal/StreambufPut.cpp) (libSceLibcInternal, NIDs `sXaxl1QGorg` and `041c37QfoUc`) - the console module was not read and no title was seen calling them. The count is a 32-bit int as the mangled names give it; the return width is not encoded and is taken as the same int. Behaviour follows [streambuf.virt.put](https://eel.is/c++draft/streambuf.virt.put) and Dinkumware's [xsputn](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/streambuf#L334-L357) in the first microsoft/STL commit: copy into the put area while it has room, otherwise pass one element to overflow, and stop at the first eof, returning the number written. Put-area room uses the end-pointer layout libc's ostreambuf_iterator assignment uses. Wide elements are 16 bits. Closes when: compared with the console module.
