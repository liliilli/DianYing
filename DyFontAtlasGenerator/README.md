# `Dy`FontAtlasGenerator

![_01](.\Docs\Readme\_01.png)

`Dy` Font Atlas generator is a font texture generator for `Dy` game-framework based on Qt and OpenGL.

You can create [SDF (Signed distance field)](https://en.wikipedia.org/wiki/Signed_distance_function) texture atlas given UTF-16 Character map codes with aribtary `.ttf` font files now.

Generated atlas information and `.png` texture altas would be used on `Dy` game framework.

## Process

| Task            | Description                                                  | Percentage |
| --------------- | ------------------------------------------------------------ | ---------- |
| DynamicCreating | Read text file and select all characters from text file as UTF-16, so dynamically creating texture atlas. | 0%         |
| About           | Creating `About` window, populating and collapsing window.   | 0%         |
| ExportBinary    | Export created information and texture buffers as independent file which can be only read on `Dy`. | 0%         |
| MultipleFont    | Create information with multiple font and multiple independent options. | 0%         |
| Concurrent      | Detach creation process from UI thread.                      | 0%         |

