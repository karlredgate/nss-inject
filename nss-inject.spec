%define revcount %(git rev-list HEAD | wc -l)
%define treeish %(git rev-parse --short HEAD)
%define localmods %(git diff-files --exit-code --quiet  || date +.m%%j%%H%%M%%S)

%define srcdir   %{getenv:PWD}

Summary: NSS Error Injection
Name: nss-inject
Version: 1.0
Release: %{revcount}.%{treeish}%{localmods}
Group: System Environment/Daemons
License: BSD
Vendor: Karl Redgate
Packager: Karl Redgate <Karl.Redgate@gmail.com>
Prefix: /

%define _topdir %(echo $PWD)/rpm
BuildRoot: %{_topdir}/BUILDROOT

%description
Error injection module for NSS.

%package devel
Summary: nss inject development files.

%description devel
Development files for NSS injection module.

%prep
%build

%install
DIR_ARGS=" -d --mode=755 "
DATA_ARGS=" --mode=644 "
PROG_ARGS=" --mode=755 "

%{__install} $DIR_ARGS $RPM_BUILD_ROOT/usr/lib64/
%{__install} $DATA_ARGS %{srcdir}/libnss_inject.so $RPM_BUILD_ROOT/usr/lib64/
ln -s libnss_inject.so $RPM_BUILD_ROOT/usr/lib64/libnss_inject.so.2

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0755)
/usr/lib64/libnss_inject.so
/usr/lib64/libnss_inject.so.2

%post
[ "$1" -gt 1 ] && {
    : Upgrading
}

[ "$1" = 1 ] && {
    : New install
}

: Done

%changelog

* Fri Oct 12 2012 Karl Redgate <Karl.Redgate@gmail.com>
- Fix for mis-wire problem after eth0 fault in active/active.

# vim:autoindent expandtab sw=4
