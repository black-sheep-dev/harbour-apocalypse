# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.32
# 

Name:       harbour-apocalypse

# >> macros
# << macros

Summary:    Apocalypse
Version:    0.3.0
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/black-sheep-dev/harbour-apocalypse
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-apocalypse.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   nemo-qml-plugin-notifications-qt5
Requires:   nemo-qml-plugin-dbus-qt5
Requires:   qt5-qtdeclarative-import-multimedia >= 5.2
Requires:   qt5-qtdeclarative-import-positioning >= 5.2
Requires:   mapboxgl-qml >= 1.7.0
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Positioning)
BuildRequires:  pkgconfig(Qt5Location)
BuildRequires:  pkgconfig(nemonotifications-qt5)
BuildRequires:  desktop-file-utils

%description
Apocalypse is an application for showing and notify of messages from german warning services like DWD, MOWAS, BIWAPP and KATWARN.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qmake5 

make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%{_datadir}/harbour-apocalypse/sounds/*
%{_datadir}/harbour-apocalypse/images/*
%{_datadir}/harbour-apocalypse/icons/*
# >> files
# << files
