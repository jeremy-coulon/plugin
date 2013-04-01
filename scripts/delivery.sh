#! /bin/bash

current_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
working_dir=$PWD
source $current_dir/config.sh
cmake_source_dir=$current_dir/..
cmake_binary_dir=$cmake_source_dir/build
cmake_install_prefix=$cmake_source_dir/install
documentation_dir=$cmake_install_prefix/share/doc/$project_name/html

# Check number of arguments
if [ $# -eq 0 ]
then
  echo "Usage: $0 version"
  exit 1
fi

# Check version argument regex
if [[ ! "$1" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]
then
  echo Error: version argument should be in the form 3.4.15.8
  exit 1
fi

# Change working directory
cd $cmake_source_dir

# Check that git working directory is clean
if [ ! -z "$(git status --porcelain)" ]
then
  echo Error: Git working directory is not clean
  exit 1
fi

if ! git fetch
then
  echo Error: git fetch
  exit 1
fi

# Check if given git tag already exist
if git tag -v v$1 > /dev/null 2>&1
then
  echo Error: git tag v$1 already exist
  exit 1
fi
if git tag -v v$1-doc > /dev/null 2>&1
then
  echo Error: git tag v$1-doc already exist
  exit 1
fi

# Save Internal Field Separator
OIFS=$IFS
IFS='.'
for v in $1
do
  version=("${version[@]}" "$v")
done
# Restore Internal Field Separator
IFS=$OIFS

# Write version.cmake
echo "cmake_minimum_required(VERSION 2.8)" > version.cmake
echo "" >> version.cmake
echo "set(\${PROJECT_NAME}_MAJOR ${version[0]})" >> version.cmake
echo "set(\${PROJECT_NAME}_MINOR ${version[1]})" >> version.cmake
echo "set(\${PROJECT_NAME}_PATCH ${version[2]})" >> version.cmake
echo "set(\${PROJECT_NAME}_TWEAK ${version[3]})" >> version.cmake
echo "" >> version.cmake
echo "set(\${PROJECT_NAME}_VERSION \"\${\${PROJECT_NAME}_MAJOR}.\${\${PROJECT_NAME}_MINOR}.\${\${PROJECT_NAME}_PATCH}.\${\${PROJECT_NAME}_TWEAK}\")" >> version.cmake

# Commit changes
git add version.cmake
git commit -m "update version.cmake"
if ! git push
then
  echo Error: git push
  exit 1
fi

# Create build directory
if [ -d $cmake_binary_dir ]
then
  rm -rf $cmake_binary_dir
fi
mkdir $cmake_binary_dir

# Create install directory
if [ -d $cmake_install_prefix ]
then
  rm -rf $cmake_install_prefix
fi
mkdir $cmake_install_prefix
mkdir -p $documentation_dir

# Prepare documentation directory
cd $documentation_dir
if ! git clone $git_remote .
then
  echo Error: git clone $git_remote .
  exit 1
fi
git checkout origin/gh-pages -b gh-pages
git branch -d master
rm -rf *

# Build + Install + Package everything
cd $cmake_binary_dir
cmake -D$project_name\_BUILD_DOC=TRUE -DCMAKE_INSTALL_PREFIX=$cmake_install_prefix $cmake_source_dir
make
make install
make package

# Commit and push documentation to remote
cd $documentation_dir
git add -A
git commit -m "Documentation for version v$1"
if ! git push
then
  echo Error: git push
  exit 1
fi

# Create documentation tag
cd $documentation_dir
git tag -s -m "Documentation for version v$1" v$1-doc
if ! git push --tags
then
  echo Error: git push --tags
  exit 1
fi

# Create delivery tag
cd $cmake_source_dir
git tag -s v$1
if ! git push --tags
then
  echo Error: git push --tags
  exit 1
fi

# Restore working directory before exiting
cd $working_dir
