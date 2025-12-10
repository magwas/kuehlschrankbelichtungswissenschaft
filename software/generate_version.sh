#!/bin/bash
set -e
GIT_TAG=$(git describe --tags --always --match "[0-9]*.[0-9]*" 2>/dev/null || echo "0.0.0")
VERSION=$(echo "$GIT_TAG" | sed -E 's/^v?([0-9]+\.[0-9]+).*$/\1/')
DISTANCE=0
BRANCH=$(git symbolic-ref --short HEAD 2>/dev/null || echo "detached")
if [[ "$GIT_TAG" =~ -([0-9]+)-g ]]; then
    DISTANCE="${BASH_REMATCH[1]}"
fi


MAJOR=$(echo "$VERSION" | cut -d. -f1)
MINOR=$(echo "$VERSION" | cut -d. -f2)
UNSIGNED=$(( ( ($MAJOR & 0xF) << 11 ) | ( ($MINOR & 0xF) << 7 ) | ($DISTANCE & 0x7F) ))

if [[ "$BRANCH" == "develop" ]]; then
    SIGN_IS_NEG=0
    SIGN_STRING=""
else
    SIGN_IS_NEG=1
    SIGN_STRING="-"
fi
cat > version.h <<EOF
#ifndef VERSION_H
#define VERSION_H

#define VERSION_STR "${SIGN_STRING}${VERSION}.${DISTANCE}"
#define SIGN_IS_NEG ${SIGN_IS_NEG}
#define UNSIGNED_CALC ${UNSIGNED}

#define BRANCH ${BRANCH}

#if SIGN_IS_NEG == 1
    #define INT_VERSION (-(UNSIGNED_CALC))
#else
    #define INT_VERSION (UNSIGNED_CALC)
#endif
#endif // VERSION_H
EOF
