Name:       harbour-apocalypse

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

# Change the version and release strings in .pro file only!

Summary:    Apocalypse
Version:    0.4.2
Release:    1
Group:      Qt/Qt
License:    GPLv2
URL:        https://github.com/black-sheep-dev/harbour-apocalypse
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   nemo-qml-plugin-configuration-qt5
Requires:   nemo-qml-plugin-notifications-qt5
#Requires:   sailfish-version >= 4.3
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(nemonotifications-qt5)
BuildRequires:  pkgconfig(keepalive)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Positioning)
BuildRequires:  pkgconfig(Qt5Location)
BuildRequires:  desktop-file-utils
BuildRequires:  qt5-qttools-linguist

%description
  Apocalypse is an application for showing and notify of messages from german warning services like DWD, MOWAS, BIWAPP and KATWARN.

  %if "%{?vendor}" == "chum"
  PackageName: Apocalypse
  Type: desktop-application
  Categories:
    - Utility
  Custom:
    DescriptionMD: https://raw.githubusercontent.com/black-sheep-dev/harbour-apocalypse/master/README.md
    Repo: https://github.com/black-sheep-dev/harbour-apocalypse/
  Icon: https://raw.githubusercontent.com/black-sheep-dev/harbour-apocalypse/master/icons/harbour-apocalypse.svg
  Screenshots:
    - https://github.com/black-sheep-dev/harbour-apocalypse/raw/master/metadata/screenshot1.png
    - https://github.com/black-sheep-dev/harbour-apocalypse/raw/master/metadata/screenshot2.png
  Url:
   Donation: https://www.paypal.com/paypalme/nubecula/1
  %endif


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original \
  --dir %{buildroot}%{_datadir}/applications \
   %{buildroot}%{_datadir}/applications/*.desktop

mkdir -p %{buildroot}%{_unitdir}
mkdir -p %{buildroot}%{_userunitdir}
mv %{buildroot}%{_datadir}/%{name}/service/%{name}.service %{buildroot}%{_userunitdir}/
rm -r %{buildroot}%{_datadir}/%{name}/service/


%files
%defattr(-,root,root,-)
%{_bindir}/*
%defattr(0644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%{_userunitdir}/%{name}.service

# >> files
# << files

%posttrans
# Remove old service (quietly)
systemctl stop %{name}.service &>/dev/null
systemctl disable %{name}.service &>/dev/null
rm -f %{_unitdir}/%{name}.service

# Install/update background daemon (default user)
systemctl-user daemon-reload
systemctl-user enable %{name}.service
systemctl-user stop %{name}.service
systemctl-user start %{name}.service
exit 0

%postun
# Run on uninstall, not on upgrade
if [ $1 -eq 0 ]; then
  systemctl-user stop %{name}.service
  systemctl-user disable %{name}.service
  systemctl-user daemon-reload

  systemctl-user reset-failed
  systemctl reset-failed
fi
exit 0
