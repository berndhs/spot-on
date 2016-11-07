%define sourceversion 2016.11.05
%define relcount 176

Name:    spot-on
Version: %{sourceversion}
Release: %{relcount}%{dist}
Summary: Spot-On Chat

License: GPLv2
Group:   Applications/Network
Packager: Bernd Stramm <bernd.stramm@gmail.com>
Url:     https://github.com/textbrowser/spot-on
%define src0dir %{name}-%{sourceversion}
%define src0 %{src0dir}.tar.gz
Source: %{src0}
Source2: %{name}.desktop
%define savedbr %{buildroot}
#BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRoot: %{_tmppath}/%{name}-%{version}

%define trunkdir branches/trunk
%define brrr %{name}-%{version}-%{release}


%define sourcedir $RPM_SOURCE_DIR
%define builddir $RPM_BUILD_DIR
%define libtarget %{_libdir}
%define bintarget %{_bindir}
%define 
%define dotpro %{name}.qt5.pro

%if X%{_vendor} == "Xredhat"
%define qmake qmake-qt4
%else
%define qmake qmake 
make
%endif

# TODO: patches (?)


#Obsoletes:
#Provides: 

BuildRequires: pkgconfig(QtGui) >= 4.8
BuildRequires: pkgconfig(qca2)
BuildRequires: ntl-devel
BuildRequires: qt-mobility-devel
BuildRequires: qt5-qtbase-devel
BuildRequires: desktop-file-utils
BuildRequires: gcc-c++
BuildRequires: sqlite-devel
BuildRequires: openssl-devel
BuildRequires: GeoIP-devel
BuildRequires: postgresql-devel
BuildRequires: lksctp-tools-devel



%define special_rpm_build_root $RPM_BUILD_ROOT

%description
An application for instant messaging 


%prep
echo "Prep starts here with %{relcount}"
echo "Prep in buildroot " %{buildroot}
pwd
cd %{sourcedir}
pwd
tar zxvf %{src0}
ls -l %{builddir}
cd %{builddir}
echo "want to copy to " %{brrr}
pwd
cp -rv %{sourcedir}/%{src0dir} $RPM_BUILD_ROOT
pwd
echo "++++++++++++++"
ls -l
echo "------------------------------------------"
cd $RPM_BUILD_ROOT
file %{src0dir}

%setup
echo "Setup"

%build
echo "Build"
echo "Building for vendor " %{_vendor}
echo "build root " %{special_rpm_build_root}
(
	pwd
	echo "qmake is " %{qmake}
	cd %{trunkdir}
	pwd
	#%{qmake} spot-on.qt5.pro
	%{qmake} %{dotpro}
	make -j1
)


%install
echo "Install part " $RPM_BUILD_ROOT

rm -rf $RPM_BUILD_ROOT
install -p -d $RPM_BUILD_ROOT%{libtarget}
install -p -m 755 libNTRU/libntru.so $RPM_BUILD_ROOT%{libtarget}
install -p -m 755 libSpotOn/libspoton.so $RPM_BUILD_ROOT%{libtarget}
install -p -d $RPM_BUILD_ROOT%{bintarget}
install -p -m 755 %{trunkdir}/Spot-On-Kernel $RPM_BUILD_ROOT%{bintarget}

install -p -d $RPM_BUILD_ROOT%{_datadir}/pixmaps
install -p -m 755 %{trunkdir}/%{name}.png $RPM_BUILD_ROOT%{_datadir}/pixmaps

desktop-file-install --dir=${RPM_BUILD_ROOT}%{_datadir}/applications %{SOURCE2}

%clean
echo "Clean:"
rm -rf %{buildroot}


%post 
echo "Post"

%postun 
echo "postun"


%files
%defattr(-,root,root,-)
#%doc README
#%doc COPYRIGHT
#%doc LICENSE.txt
%{_bindir}/Spot-On-Kernel
%{_datadir}/applications/%{name}.desktop
#%{libtarget}/libntru.so
%{libtarget}/libspoton.so
%{_datadir}/pixmaps/%{name}.png

%changelog




* Fri Oct 28 2016 Bernd Stramm <bernd.stramm@gmail.com> - 0.1.1-1
- Initial RPM build

