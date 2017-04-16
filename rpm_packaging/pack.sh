#!/bin/bash -x
EXE_NAME=spot-on
PROJ_NAME=spot-on
CHANGELOG=${EXE_NAME}.changes
DESKTOP=${EXE_NAME}.desktop
#VERSION=`grep "ProgramVersion::VersionNumber" src/version.cpp \
#        | awk '{print $3;}' \
#        | sed s/[\(\"\;\)]//g`
#VERSION=`version.sh`
VERSION=`grep VERSION ../Common/spot-on-common.h "\
	| awk '{print $3}' | sed s/\"//g`

	
PACKDIR=${HOME}/packaging/${PROJ_NAME}

makearchive.sh ${EXE_NAME}-${VERSION} master
cp ${EXE_NAME}-${VERSION}.tar.gz ${PACKDIR}
cp ${CHANGELOG} ${PACKDIR}
#cp ${DESKTOP} ${PACKDIR}
echo ${EXE_NAME} > ${PACKDIR}/pack-name
echo ${VERSION} > ${PACKDIR}/pack-version
ls -l ${PACKDIR}/${EXE_NAME}-${VERSION}.tar.gz
ls -l ${PACKDIR}/pack-*

if [ x$1 == "xmake" ]
then
  cd ${PACKDIR}
  make
fi

