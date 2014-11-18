%define revcount %(git rev-list HEAD | wc -l)
%define treeish %(git rev-parse --short HEAD)
%define localmods %(git diff-files --exit-code --quiet  || date +.m%%j%%H%%M%%S)

Summary: NSS Error Injection
Name: nss-inject
Version: 7.0
Release: %{revcount}.%{treeish}%{localmods}
Group: System Environment/Daemons
License: BSD
Vendor: Karl Redgate
Packager: Karl N. Redgate <Karl.Redgate@gmail.com>
Prefix: /
%define _topdir %(echo $PWD)/rpm
BuildRoot: %{_topdir}/BUILDROOT
%define Exports %(echo $PWD)/exports

# require glibc and NetworkManager because %post modifies conf files
# provided by those packages.
# (Better would be to do the modification in %triggerin scripts.)
Requires(post): coreutils
Requires(post): glibc
Requires(post): grep
Requires(post): NetworkManager
Requires(post): sed

%description
Error injection module for NSS.

%package devel
Summary: nss inject development files.

%description devel
Development files for NSS injection module.

%prep
%build

%install
tar -C %{Exports} -cf - . | (cd $RPM_BUILD_ROOT; tar xf -)

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0755)
/usr/lib64/

%files devel
/usr/include

%post
[ "$1" -gt 1 ] && {
    : Upgrading
}

[ "$1" = 1 ] && {
    : New install
    sqlite3 /var/db/hosts.db < /usr/share/avance-network/hosts.sql
    # sed --in-place -e '/^host/chosts: sqlite files inject dns' /etc/nsswitch.conf
}

: Done

%changelog

* Fri Oct 12 2012 Karl Redgate <Karl.Redgate@gmail.com>
- Fix for mis-wire problem after priv0 fault in active/active.

# vim:autoindent
# vim:syntax=plain
