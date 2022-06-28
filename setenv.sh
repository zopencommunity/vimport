#!/bin/sh
#
# Set up environment variables for general build tool to operate
#
if ! [ -f ./setenv.sh ]; then
  echo "Need to source from the setenv.sh directory" >&2
  return 0
fi

export PORT_ROOT="${PWD}"
export PORT_TYPE="GIT"

export PORT_GIT_URL="https://github.com/vim/vim.git"
export PORT_GIT_DEPS="git make"
export PORT_EXTRA_CONFIGURE_OPTS="--with-features=big --without-x --enable-gui=no"
