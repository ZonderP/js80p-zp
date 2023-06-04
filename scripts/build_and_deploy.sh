#!/bin/bash


set -e

BASE_DIR=~

FST_DIRS_LINUX_32="$BASE_DIR/vst"
FST_DIRS_LINUX_64="$BASE_DIR/vst"

FST_DIRS_WINE_32="$BASE_DIR/.wine/drive_c/vst"
FST_DIRS_WINE_64="$BASE_DIR/.wine/drive_c/vst"

VST3_DIRS_LINUX_32="$BASE_DIR/vst"
VST3_DIRS_LINUX_64="$BASE_DIR/vst"

VST3_DIRS_WINE_32="$BASE_DIR/.wine/drive_c/vst $BASE_DIR/.wine/drive_c/Program*Files/Common*Files/VST3"
VST3_DIRS_WINE_64="$BASE_DIR/.wine/drive_c/vst $BASE_DIR/.wine/drive_c/Program*Files/Common*Files/VST3"


main()
{
    local plugin_type="$1"
    local target_os="$2"
    local arch="$3"
    local target_platform=""
    local built_plugin=""

    if [[ "$plugin_type$target_os$arch" = "" ]]
    then
        echo "Usage: $0 fst|vst3 linux|windows 64bit|32bit" >&2
        return 1
    fi

    if [[ "$plugin_type" = "" ]]; then plugin_type="fst"; fi
    if [[ "$target_os" = "" ]]; then target_os="linux"; fi
    if [[ "$arch" = "" ]]; then arch="64bit"; fi

    case "$arch" in
        "32bit") target_platform="i686" ;;
        "64bit") target_platform="x86_64" ;;
        *)
            echo "Unknown architecture: \"$arch\" - should be either \"32bit\" or \"64bit\"" >&2
            return 1
            ;;
    esac

    case "$target_os" in
        "linux") target_platform="$target_platform-gpp" ;;
        "windows") target_platform="$target_platform-w64-mingw32" ;;
        *)
            echo "Unknown target OS: \"$target_os\" - should be either \"linux\" or \"windows\"" >&2
            return 1
            ;;
    esac

    built_plugin="dist/js80p-dev-$target_os-$arch-$plugin_type"

    case "$target_os-$plugin_type" in
        "linux-fst") built_plugin="$built_plugin/js80p.so" ;;
        "windows-fst") built_plugin="$built_plugin/js80p.dll" ;;
        "linux-vst3"|"windows-vst3") built_plugin="$built_plugin/js80p.vst3" ;;
        *)
            echo "Unknown plugin type: \"$plugin_type\" - should be either \"fst\" or \"vst3\"" >&2
            return 1
            ;;
    esac

    echo "Building; plugin_type=\"$plugin_type\", target_os=\"$target_os\", arch=\"$arch\"" >&2

    TARGET_PLATFORM="$target_platform" make

    case "$target_os-$plugin_type-$arch" in
        "linux-fst-64bit")      replace_in_dir "$built_plugin" $FST_DIRS_LINUX_64 ;;
        "linux-vst3-64bit")     replace_in_dir "$built_plugin" $VST3_DIRS_LINUX_64 ;;
        "windows-fst-64bit")    replace_in_dir "$built_plugin" $FST_DIRS_WINE_64 ;;
        "windows-vst3-64bit")   replace_in_dir "$built_plugin" $VST3_DIRS_WINE_64 ;;
        "linux-fst-32bit")      replace_in_dir "$built_plugin" $FST_DIRS_LINUX_32 ;;
        "linux-vst3-32bit")     replace_in_dir "$built_plugin" $VST3_DIRS_LINUX_32 ;;
        "windows-fst-32bit")    replace_in_dir "$built_plugin" $FST_DIRS_WINE_32 ;;
        "windows-vst3-32bit")   replace_in_dir "$built_plugin" $VST3_DIRS_WINE_32 ;;
    esac

    echo "SUCCESS" >&2

    return 0
}


replace_in_dir()
{
    local built_plugin="$1"
    local dir

    shift

    while [[ "$1" != "" ]]
    do
        dir="$1"
        echo ""
        echo "Replacing JS80P; dir=\"$dir\""
        rm -fv "$dir"/js80p.*
        cp -v "$built_plugin" "$dir/"
        shift
    done
}


main "$@"
