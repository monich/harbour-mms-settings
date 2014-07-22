%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Name:       harbour-mms-settings
Summary:    Advanced MMS settings
Version:    1.0.2
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        http://jolla.com/
Source0:    %{name}-%{version}.tar.bz2

Requires:      sailfishsilica-qt5 >= 0.10.9
BuildRequires: pkgconfig(mlite5) >= 0.2.4
BuildRequires: pkgconfig(glib-2.0) >= 2.32
BuildRequires: pkgconfig(sailfishapp) >= 1.0.2
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: desktop-file-utils

%description
Advanced MMS settings

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original \
  --dir %{buildroot}%{_datadir}/applications \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png

%changelog
* Fri Jun 13 2014 Slava Monich <slava.monich@jolla.com> 1.0.0
- Initial version
