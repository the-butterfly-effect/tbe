# Increase mainrel when increasing the rev number
%define rev     git41a1e39
%define mainrel 1
# Increase snaprel when rebuilding with the same rev number
%define snaprel 1

Name:           tbe
Version:        0.9
# Latest upstream release is Milestone 9 (0.9).
# We use the release tag to indicate that we use a snapshot newer than 0.9.
Release:        %mkrel %{mainrel}.%{rev}.%{snaprel}
Summary:        The Butterfly Effect is a physics-based puzzle game
Group:          Games/Puzzles
License:        GPLv2
URL:            https://github.com/kaa-ching/tbe
Source0:        %{name}-%{version}-%{rev}.tar.gz
# https://www.transifex.com/projects/p/thebutterflyeffect
Source1:        %{name}-i18n-%{version}-%{rev}.tar.gz
# https://www.transifex.com/projects/p/thebutterflyeffect
Patch0:         tbe-0.9-mga-fix-ndebug.patch
Patch1:         tbe-0.9-mga-datadir.patch
BuildRequires:  imagemagick
BuildRequires:  cmake
BuildRequires:  qt4-devel
BuildRequires:  gettext
Provides:       thebutterflyeffect = %{version}-%{release}

%description
The Butterfly Effect uses realistic physics simulations to combine
lots of simple mechanical elements to achieve a simple goal in the
most complex way possible.

%prep
%setup -q -n %{name}-%{version}-%{rev}
tar -xf %{SOURCE1}
%patch0 -p1
%patch1 -p1

%build
pushd i18n
./%{name}_levels_i18n.sh
popd
# Use dummy installation directory to prevent heavy patching
# of the bad make install instructions
%cmake -DCMAKE_INSTALL_PREFIX=/tmp \
       -DBUILD_SHARED_LIBS=OFF
%make

%install
# Install to buildroot/tmp, and then move dir to the buildroot
%makeinstall_std -C build

pushd %{buildroot}/tmp
install -d %{buildroot}%{_gamesdatadir}/%{name}
cp -a i18n images levels %{buildroot}%{_gamesdatadir}/%{name}
install -D -m755 bin/%{name} %{buildroot}%{_gamesbindir}/%{name}
popd
rm -rf %{buildroot}/tmp

pushd imagery/illustrations
mkdir -p %{buildroot}%{_iconsdir}/hicolor/{128x128,64x64,48x48,32x32,16x16}/apps
convert -scale 16x16 %{name}-icon.ico %{buildroot}%{_iconsdir}/hicolor/16x16/apps/%{name}.png
convert -scale 32x32 %{name}-icon.ico %{buildroot}%{_iconsdir}/hicolor/32x32/apps/%{name}.png
convert -scale 48x48 %{name}-icon.ico %{buildroot}%{_iconsdir}/hicolor/48x48/apps/%{name}.png
convert -scale 64x64 %{name}-icon.ico %{buildroot}%{_iconsdir}/hicolor/64x64/apps/%{name}.png
convert -scale 128x128 %{name}-icon.ico %{buildroot}%{_iconsdir}/hicolor/128x128/apps/%{name}.png
popd

install -D -m644 installer/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop

%files
%doc AUTHORS DESCRIPTION README.md installer/License
%{_datadir}/applications/%{name}.desktop
%{_gamesbindir}/%{name}
%{_gamesdatadir}/%{name}
%{_iconsdir}/hicolor/*/apps/%{name}.png
