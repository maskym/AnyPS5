# libSceLibcInternal

## remove | `_ZNSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE4_RepES3_cm` | `1dY2KJfkgMM` |

## remove | `_ZNSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE4_RepES3_wm` | `6QU40olMkOM` |

## remove [_ZNKSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE4_PutES3_PKcm]

## remove [_ZNKSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE4_RepES3_cm]

## remove [_ZNKSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE4_PutES3_PKwm]

## remove [_ZNKSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE4_RepES3_wm]

## add Unknown function info

- [num_put and money_put _Put and _Rep](../../../core/libs/prx/libSceLibcInternal/FacetPut.cpp) (libSceLibcInternal, NIDs `k8zgjeBmpVY`, `tCihLs4UJxQ`, `kRGVhisjgMg`, `-b+Avqa2v9k`, `1dY2KJfkgMM` and `6QU40olMkOM`) - the console module was not read and no title was seen calling them. Arguments come from the mangled names: the `num_put` members are const and take the facet first, and the `money_put` `_Rep` is static. Behaviour follows Dinkumware's code in the first microsoft/STL commit ([xlocnum](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocnum#L1542-L1557), [xlocmon](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocmon#L875-L881)): each element goes through the `ostreambuf_iterator` assignment libc's facets use, which sets the failed flag on a null buffer or a refused overflow and keeps writing after it. Wide elements are 16 bits. Closes when: compared with the console module.
