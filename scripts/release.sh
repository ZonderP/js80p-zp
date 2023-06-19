#!/bin/bash

# set -x
set -e
set -u
set -o pipefail

TARGET_PLATFORMS="x86_64-w64-mingw32 i686-w64-mingw32 x86_64-gpp i686-gpp"
PLUGIN_TYPES="fst vst3"
TEXT_FILES="LICENSE.txt README.txt NEWS.txt"

main()
{
    local version_tag
    local version_str
    local version_int
    local target_platform
    local source_dir
    local source_archive
    local dist_dir
    local dist_archive
    local uncommitted
    local date
    local src_file
    local dst_file

    log "Verifying repository"

    if [[ ! -d "src" ]] || [[ ! -d "presets" ]]
    then
        error "This script must be run in the root directory of the repository."
    fi

    version_tag="$(git tag --points-at HEAD | grep -E "^v[0-9]+\\.[0-9]\\.[0-9]$" || true)"

    if [[ "$version_tag" = "" ]]
    then
        version_tag="v999.0.0"
        log "WARNING: No version tag (format: \"v123.4.5\") found on git HEAD, creating a development release."
    fi

    if [[ "$version_tag" != "v999.0.0" ]]
    then
        uncommitted="$(git status --porcelain | wc -l)"

        if [[ $uncommitted -ne 0 ]]
        then
            error "Commit your changes first. (Will not release $version_tag from a dirty repo.)"
        fi

        date="$(date '+%Y-%m-%d')"

        grep "^$(echo "$version_tag" | sed 's/\./\\./g') ($date)" NEWS.txt >/dev/null \
            || error "Cannot find '$version_tag ($date)' in NEWS.txt"
    fi

    version_str="${version_tag:1}"
    version_int="$(version_str_to_int "$version_str")"
    version_as_file_name="$(version_str_to_file_name "$version_str")"
    source_dir="js80p-$version_as_file_name-src"
    source_archive="$source_dir.zip"

    log "Cleaning up"

    cleanup "dist"
    cleanup "dist/$source_dir"

    log "Copying source"

    mkdir -p "dist/$source_dir"
    cp --verbose --recursive \
        build.bat \
        build.ps1 \
        build.sh \
        Doxyfile \
        gui \
        js80p.png \
        lib \
        LICENSE.txt \
        make \
        Makefile \
        pm-fm-equivalence.md \
        presets \
        README.md \
        README.txt \
        scripts \
        src \
        tests \
        NEWS.txt \
        "dist/$source_dir/"

    find "dist/$source_dir/" -name ".*.swp" -delete

    log "Creating source archive"

    cd dist
    zip --recurse-paths -9 "$source_archive" "$source_dir"
    cd ..

    log "Running unit tests"

    call_make "x86_64-w64-mingw32" "$version_str" check

    for target_platform in $TARGET_PLATFORMS
    do
        log "Building for target: $target_platform"

        call_make "$target_platform" "$version_str" clean
        call_make "$target_platform" "$version_str" all

        for plugin_type in $PLUGIN_TYPES
        do
            dist_dir="$(call_make "$target_platform" "$version_str" show_dist_dir_prefix)-$plugin_type"
            dist_dir="$(basename "$dist_dir")"
            dist_archive="$dist_dir.zip"

            log "Copying presets, etc. to dist/$dist_dir"

            cp --verbose --recursive presets "dist/$dist_dir/"

            if [[ "$target_platform" =~ "-mingw32" ]]
            then
                for src_file in $TEXT_FILES
                do
                    dst_file="dist/$dist_dir/$src_file"
                    printf "Converting %s to %s\n" "$src_file" "$dst_file"
                    cat "$src_file" | sed 's/$/\r/g' >"$dst_file"
                done
            else
                cp --verbose $TEXT_FILES "dist/$dist_dir/"
            fi

            log "Creating release archive: dist/$dist_archive"

            cd dist
            zip --recurse-paths -9 "$dist_archive" "$dist_dir"
            cd ..
        done
    done

    log "Done"
}

error()
{
    local message_tpl="$1"

    shift

    printf "ERROR: $message_tpl\n" "$@" >&2

    exit 1
}

log()
{
    local message_tpl="$1"

    shift

    printf "### %s\t$message_tpl\n" "$(date)" "$@" >&2
}

cleanup()
{
    local name="$1"

    if [[ "$name" = "" ]] || [[ "${name:0:1}" = "/" ]]
    then
        error "Invalid argument for cleanup: %s" "$name"
    else
        log "Removing $name"
        rm -rf -- "$name"
    fi
}

version_str_to_int()
{
    local version_str="$1"

    printf "%s0\n" "$version_str" | sed "s/[^0-9]//g"
}

version_str_to_file_name()
{
    local version_str="$1"

    printf "%s\n" "$version_str" | sed "s/[^0-9]/_/g"
}

call_make()
{
    local target_platform="$1"
    local version_str="$2"
    local version_int
    local version_as_file_name

    shift
    shift

    version_int="$(version_str_to_int "$version_str")"
    version_as_file_name="$(version_str_to_file_name "$version_str")"

    TARGET_PLATFORM="$target_platform" \
        VERSION_STR="$version_str" \
        VERSION_INT="$version_int" \
        VERSION_AS_FILE_NAME="$version_as_file_name" \
        make "$@"
}

main "$@"
