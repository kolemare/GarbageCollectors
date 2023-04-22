#!/bin/bash

apply_lasi=false
apply_webp=false
revert=false

while [[ $# -gt 0 ]]; do
  case "$1" in
    -lasi)
      apply_lasi=true
      shift
      ;;
    -webp)
      apply_webp=true
      shift
      ;;
    -revert)
      revert=true
      shift
      ;;
    *)
      echo "Unrecognized flag: $1"
      echo "Usage: apply_changes.sh [-lasi] [-webp] [-revert]"
      exit 1
      ;;
  esac
done

cd application/include/graphviz

if [ "$revert" == "true" ]; then
  revert_plugin_changes lasi
  revert_plugin_changes webp
else
  if [ "$apply_lasi" == "true" ]; then
    apply_plugin_changes lasi gvplugin_lasi
  fi

  if [ "$apply_webp" == "true" ]; then
    apply_plugin_changes webp gvplugin_webp
  fi
fi

exit 0

apply_plugin_changes() {
  plugin_name=$1
  target_name=$2
  echo "Applying changes to $plugin_name plugin..."

  echo "Modifying cmd/dot/CMakeLists.txt..."
  sed "/target_link_libraries(dot_builtins PRIVATE $target_name)/d" cmd/dot/CMakeLists.txt > temp.txt
  mv temp.txt cmd/dot/CMakeLists.txt

  echo "Modifying plugin/CMakeLists.txt..."
  sed "/add_subdirectory($plugin_name)/d" plugin/CMakeLists.txt > temp.txt
  mv temp.txt plugin/CMakeLists.txt

  echo "Changes applied successfully for $plugin_name plugin."
}

revert_plugin_changes() {
  plugin_name=$1
  echo "Reverting changes to $plugin_name plugin..."
  git reset --hard HEAD
  echo "Changes reverted successfully for $plugin_name plugin."
}