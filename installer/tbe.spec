Name:           tbe
Version:        0.9.2.1
Release:        %mkrel 1
Summary:        The Butterfly Effect is a physics-based puzzle game
Group:          Games/Puzzles
License:        GPLv2
URL:            http://kaa-ching.github.io/tbe
# https://github.com/kaa-ching/tbe/archive/v%{version}.tar.gz
Source0:        %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  qt4-devel
BuildRequires:  gettext
Provides:       thebutterflyeffect = %{version}-%{release}

%description
The Butterfly Effect uses realistic physics simulations to combine
lots of simple mechanical elements to achieve a simple goal in the
most complex way possible.

%prep
%setup -q

%build
pushd i18n
./%{name}_levels_i18n.sh
popd
%cmake -DWITH_DOCS=OFF \
       -DBUILD_SHARED_LIBS=OFF \
       -DCMAKE_BUILD_TYPE=Release
%make

%install
%makeinstall_std -C build

%files
%doc AUTHORS DESCRIPTION README.md installer/License imagery/README.*
%{_datadir}/applications/%{name}.desktop
%{_gamesbindir}/%{name}
%{_gamesdatadir}/%{name}
%{_iconsdir}/hicolor/*/apps/%{name}.png
