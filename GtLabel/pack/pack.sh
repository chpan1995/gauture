#!/bin/bash

cqtdp=`which cqtdeployer`
if [ ! $cqtdp ];then
	echo "Err: Please install cqtdeployer"
	exit 1
fi

qmake_bin=`which qmake`
if [ ! $qmake_bin ];then
	echo "Err: Please install qmake"
	exit 2
fi

if [ $# -ne 1 ];then
	echo "Usage: $0 <package version>"
	exit 3
fi

# if [ $# -ne 2 ];then
# 	echo "Usage: $0 <package version> <0/1>(0: autostart on websocket, 1: autostart on ui)"
# 	exit 3
# fi

set -e

cpu_num=`cat /proc/cpuinfo | grep processor | wc -l`
cpu_num=$((${cpu_num} - 1))

pack_src_dir=$(cd $(dirname $0);pwd)
repo_dir=$(cd $pack_src_dir/..;pwd)
repo_build_dir=$repo_dir/build
pack_dir=$repo_build_dir/$1
pack_label_dir=$pack_dir/opt/appGtLabel

bin_dir=$pack_label_dir/bin
lib_dir=$pack_label_dir/lib

rm -rf $repo_build_dir
mkdir -p $pack_label_dir
mkdir -p ${bin_dir}/logs
mkdir -p ${lib_dir}

# 拷贝基础文件
cp -rf $pack_src_dir/DEBIAN $pack_dir
cp $pack_src_dir/appGtLabel.desktop $pack_label_dir
cp $pack_src_dir/icon.png $pack_label_dir
# 打包软件配置文件项
soft_conf="icon.png"
for sconf in ${soft_conf}
do
	if [ -e ${pack_src_dir}/${sconf} ];then
		cp -rf ${pack_src_dir}/${sconf} ${bin_dir}
	fi
done


qmake_bin=/opt/qt/6.8.2/gcc_64/bin/qmake


# 编译update程序
cd $repo_build_dir
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_PREFIX_PATH:PATH=/opt/qt/6.8.2/gcc_64 -DBUILD_SHARED_LIBS=OFF ../update;make -j${cpu_num}
cp -rf $repo_build_dir/Gzupdate $bin_dir
cp -rf $pack_src_dir/Gzupdate.sh $pack_label_dir
rm -rf CMakeCache.txt

##############################################################
#################### 编译appGtLabel，并打包依赖库 ####################
##############################################################
cd $repo_build_dir
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_PREFIX_PATH:PATH=/opt/qt/6.8.2/gcc_64 ..;make -j${cpu_num}
$cqtdp -bin $repo_build_dir/appGtLabel -qmake $qmake_bin -qmlDir $repo_dir/UI -targetDir $pack_label_dir

cp -rf $repo_build_dir/Gzjump $bin_dir

GtLabel_bin=$bin_dir/appGtLabel
GtLabel_deplist=$(ldd $GtLabel_bin | awk '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $GtLabel_deplist $lib_dir


# 更新相关文件的版本号
verkeyword=Version
# desktop文件
desktop_vkeyword=$(sed -n /${verkeyword}/p ${pack_src_dir}/appGtLabel.desktop)
(sed "/${desktop_vkeyword}/c Version=$1" ${pack_src_dir}/appGtLabel.desktop) > ${pack_label_dir}/tmp_version.desktop
mv ${pack_label_dir}/tmp_version.desktop ${pack_label_dir}/appGtLabel.desktop

# control文件
# version keyword
control_vkeyword=$(sed -n /${verkeyword}/p ${pack_src_dir}/DEBIAN/control)

# install size keyword
control_skeyword=$(sed -n /Installed-Size/p ${pack_src_dir}/DEBIAN/control)
# real size
isize=$(du -d 1 ${pack_dir} | awk '{if (match($2,"opt")){printf("%s", $1)} }')

# 更新version 和 install size
((sed "/${control_vkeyword}/c Version: $1" ${pack_src_dir}/DEBIAN/control) \
	| sed "/${control_skeyword}/c Installed-Size: ${isize}") > $pack_dir/DEBIAN/control

# 生成dep文件
cd $repo_build_dir;dpkg -b $1 appGtLabel$1.deb
