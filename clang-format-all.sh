ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
SRC_DIR=${ROOT_DIR}/src
CLANG_FORMAT=${CLANG_FORMAT:-clang-format}

# Double clang-format, as it seems that one pass isn't always enough
find ${SRC_DIR} -iname "*.h" -o -iname "*.cpp" | xargs ${CLANG_FORMAT} -i -style=file
find ${SRC_DIR} -iname "*.h" -o -iname "*.cpp" | xargs ${CLANG_FORMAT} -i -style=file