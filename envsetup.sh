export ARCH=mips
export CROSS_COMPILE=mips-openwrt-linux-musl-
export LEDE=/home/avware/lede/trunk

export STAGING_DIR=$LEDE/staging_dir/toolchain-mips_24kc_gcc-5.4.0_musl-1.1.16
export KBUILD=$LEDE/build_dir/target-mips_24kc_musl-1.1.16/linux-lantiq_xrx200/linux-4.4.42

export PATH=$PATH:$STAGING_DIR/bin
