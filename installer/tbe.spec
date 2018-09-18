Name:           tbe
Version:        0.9.3.1
Release:        %mkrel 1
Summary:        The Butterfly Effect is a physics-based puzzle game
Group:          Games/Puzzles
License:        GPLv2
URL:            http://the-butterfly-effect.org
Source0:        https://github.com/the-butterfly-effect/tbe/archive/v%{version}/%{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  qt5-devel
BuildRequires:  gettext
Provides:       thebutterflyeffect = %{version}-%{release}

%description
The Butterfly Effect uses realistic physics simulations to combine
lots of simple mechanical elements to achieve a simple goal in the
most complex way possible.

%prep
%setup -q

%build
%cmake -DWITH_DOCS=OFF \
       -DBUILD_SHARED_LIBS=OFF \
       -DCMAKE_BUILD_TYPE=Release
%make

%install
%make_install -C build

%files
%doc AUTHORS DESCRIPTION README.md installer/License imagery/README.*
%{_datadir}/applications/%{name}.desktop
%{_gamesbindir}/%{name}
%{_gamesdatadir}/%{name}
%{_iconsdir}/hicolor/*/apps/%{name}.png
