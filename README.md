**FlexArch**

An attemp to describe universal archive/fs interface and implement sample applications using it.

Folders:
- FlexArch - common directory, could be used for development new plugins or help to use existing plugins
- FlexArchCl - command-line utility
- FlexArchGui - GUI utility implemented with wxWidgets
- FlexArchPlugin_zip - sample plugin to access ZIP archives, using libzip

**Termins**

- Plugin - dynamic library, which implements described set of functiond and could be used to provide support for new file formats. Should always be named like `FlexArchPlugin_*.so` or `FlexArchPlugin_*.dll`, where \* is archive format extension or archive format name
- CL - command line interface, implemented in FlexArchCl

**How to use**

*(It isn't done yet, so this section sould be read as "how to use it when it becode usable")*

Can be used as archivator for everyday work using GUI or CL interfaces, as well as can be used for automatization work with archives (for example, applying patches) using CL version only. 

It can be used to access some higly-specific archives like proprioretary formats (for example, packed game files). The main reason to choose this solution - ready to use interfaces, you only need to write plugin for you file format.

You can use this code and plugins in your own solution to work with some files. Well-described and standartized interface will make your solution portable, easy to implement and able to fast switch from one format to another.

**Instruction for creating plugins**

All functions that should be implemented are described in `FlexArch/plugin.h`, and examples of implementation could be found in FlexArchPlugin_zip. If something unclear - create an issue with question or ask me directly, my goal is to describe everythig as clean as possible.