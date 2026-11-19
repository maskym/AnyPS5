# libSceFreeType

## change [FT_Stroker_ExportBorder, FT_Stroker_Export]

- [FT_Stroker_ExportBorder, FT_Stroker_Export](../../../core/libs/prx/libSceFreeType/src/Stroker.cpp) (libSceFreeType) - console signatures are unverified and the console's FreeType version is unknown. The wrapper forwards to bundled [FreeType 2.15.0](https://github.com/freetype/freetype/blob/37515cf56ba3349656049edbed89ffc6f6466f2d/src/base/ftstroke.c#L2013-L2041), which ignores a null stroker or outline and adds to unsigned 16-bit counts; [FreeType 2.4.12](https://github.com/freetype/freetype/blob/cecf93ef90c660a8b0b45e5adbbfb5ea443fb6b9/src/base/ftstroke.c#L2008-L2022) has no null checks and [signed 16-bit counts](https://github.com/freetype/freetype/blob/cecf93ef90c660a8b0b45e5adbbfb5ea443fb6b9/src/base/ftstroke.c#L700-L748). On Windows (32-bit `FT_Pos`) FreeType writes 32-bit points that are then widened in place; a border of 65536 or more points can stay partly 32-bit

# libSceFreeTypeOt

## change [FT_Stroker_Export](../../../core/libs/prx/libSceFreeTypeOt/Export.cpp)

- [FT_Stroker_Export](../../../core/libs/prx/libSceFreeTypeOt/Export.cpp) (libSceFreeTypeOt, NID `2rcvKZfm5rI`) - the console prototype is unverified and this provider reports FreeType 2.4.11. It forwards to libSceFreeType's `FT_Stroker_Export`, so it follows bundled FreeType as that library's FT_Stroker_ExportBorder, FT_Stroker_Export entry describes. [2.4.11](https://github.com/freetype/freetype/blob/c6a66b49e64e9a21c013f23c867d238cf1aa98cb/src/base/ftstroke.c#L2008-L2033) exports both borders as 2.4.12 does: it has no null checks and adds to signed 16-bit counts, where the bundled source ignores a null stroker or outline and adds to unsigned counts.

## change [FT_Stroker_ExportBorder and FT_Stroker_ParseOutline]

- [FT_Stroker_ExportBorder and FT_Stroker_ParseOutline](../../../core/libs/prx/libSceFreeTypeOt/ForwardExports.cpp) (`7cIS2cFG3PE`, `RvFx7dv9ThU`) resolve to the libSceFreeType wrappers like the other stroker exports here, so the console's prototypes for this provider are assumed to be FreeType's, which are unchanged from 2.4.11 to 2.15.0. Output is that of the bundled FreeType 2.15.0, not of the provider's dated 2.4.11-2.4.12 code, and is not compared with a console. On Windows `FT_Stroker_ExportBorder` widens the 32-bit points it writes, as [the libSceFreeType wrapper](libSceFreeType.md) does with a 32-bit native `FT_Pos`.
