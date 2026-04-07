#!/usr/bin/env bash
set -e

# THIS SCRIPT IS CALLED FROM SNAPCRAFT BUILD TO PACK A NODE.JS APP.
# PLEASE SEE CORRESPONDING SNAPCRAFT.YAML FOR DETAILS. 

 # detect target arch
if [ $CRAFT_ARCH_BUILD_FOR == "arm64" ]; then
  target_arch="arm64"
else 
  target_arch="x64"
fi

# detect build arch
CRAFT_BUILD_ARCH=$(dpkg --print-architecture)
if [ $CRAFT_BUILD_ARCH == "arm64" ]; then
  build_arch="arm64"
else 
  build_arch="x64"
fi

# We use a Node.js runtime cache to prevent downloading on any Node.js app build.
node_cache_dir="${SNAPCRAFT_PROJECT_DIR}/../../nodejs/.cache"
node_to_pack_cache_dir="${node_cache_dir}/v${NODE_VERSION}/${target_arch}"
node_path="${node_to_pack_cache_dir}/bin"

# fetch node for target arch to be packed
if [ ! -f "$node_to_pack_cache_dir/bin/node" ]; then
  node_uri="https://nodejs.org/dist/v${NODE_VERSION}/node-v${NODE_VERSION}-linux-${target_arch}.tar.gz"
  echo downloading node to pack: $node_uri
  mkdir -p $node_to_pack_cache_dir
  curl $node_uri | tar xzf - -C $node_to_pack_cache_dir --no-same-owner --strip-components=1
else
  echo "node (pack) cache hit: v${NODE_VERSION} (${target_arch})"
fi
cp -r $node_path ${CRAFT_PART_INSTALL}

# fetch node used for build (if not equal to target arch for x-build)
if [ $build_arch != $target_arch ]; then
  node_for_build_cache_dir="${node_cache_dir}/v${NODE_VERSION}/${build_arch}" 
  if [ ! -f "$node_for_build_cache_dir/bin/node" ]; then
    node_uri="https://nodejs.org/dist/v${NODE_VERSION}/node-v${NODE_VERSION}-linux-${build_arch}.tar.gz"
    echo downloading node for x-build: $node_uri
    mkdir -p $node_for_build_cache_dir
    curl $node_uri | tar xzf - -C $node_for_build_cache_dir --no-same-owner --strip-components=1
  else
    echo "node (build) cache hit: v${NODE_VERSION} (${build_arch})"
  fi
  node_path="${node_for_build_cache_dir}/bin"
fi

# set the path to build target node to be used for build
export PATH=$node_path:$PATH
echo "node (build): $(node --version)"
echo "node (build): $node_path"

# adapt the file dependencies in package.json to current working directory
find $CRAFT_PART_BUILD -name package.json -type f -exec sed -i "s|$file:\.\.\/\.\./nodejs|$file:${SNAPCRAFT_PROJECT_DIR}/../../nodejs|g" {} \;

# install and compile (tsc -> ./dist)
echo "npm install ..."
npm install --no-fund
npm run tsc

# pack and install the app (only production)
echo "npm pack ..."
tarball=$(npm pack . | tail -1)
npm install -g --prefix $CRAFT_PART_INSTALL $tarball --ignore-scripts --omit=dev --no-fund

# remove ctrlx-datalayer prebuilds of unused archs to reduce snap size
echo "pruning ..."
package_name=$(npm run getName -s)
prebuilds_location="${CRAFT_PART_INSTALL}/lib/node_modules/${package_name}/node_modules/ctrlx-datalayer/prebuilds"
if [ -d "$prebuilds_location" ]; then
  find $prebuilds_location -type f -not -ipath "*/linux-${target_arch}*" -delete
fi

# remove unused binaries
rm -rf ${CRAFT_PART_INSTALL}/bin/npm
rm -rf ${CRAFT_PART_INSTALL}/bin/npx
rm -rf ${CRAFT_PART_INSTALL}/bin/corepack

# remove unused node modules
rm -rf ${CRAFT_PART_INSTALL}/lib/node_modules/npm
rm -rf ${CRAFT_PART_INSTALL}/lib/node_modules/corepack