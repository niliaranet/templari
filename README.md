# Templari
FZF wrapper for managing templates.

## Usage
```bash
templari -h # display help

templari # open template picker
templari <directory/file> # create new template from the specified directory/file
```

Example usage:
```bash
mkdir template
touch template/a template/b template/c

templari template/ # turns the folder into a template
templari # open it in the file picker to get the contents back
```

The templates' saving location is `$HOME/.config/templari/`.

## Compiling
```bash
git clone github.com/niliaranet/templari
cd templari
make
```

The binary file (templari) should be stored somewhere in $PATH afterwards.
