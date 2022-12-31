# Copyright 1999-2021 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=8
inherit cmake

DESCRIPTION="Gamestream client for embedded systems"
HOMEPAGE="https://github.com/moonlight-stream/moonlight-embedded"
SRC_URI="https://github.com/moonlight-stream/moonlight-embedded/releases/download/v${PV}/moonlight-embedded-${PV}.tar.xz"
S="${WORKDIR}"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="arm64"

RDEPEND="
	>=dev-libs/openssl-1.1.1l:0/1.1
	>=media-libs/libsdl2-2.0.16-r1:0[kms]
	>=media-libs/libva-2.12.0:0
	>=media-libs/opus-1.3.1-r2:0
	>=media-libs/sdl2-ttf-2.0.15:0
	>=media-sound/pulseaudio-15.0-r1:0
	>=media-video/ffmpeg-4.4-r1:0
	>=net-dns/avahi-0.8-r5:0
"
DEPEND="${RDEPEND}"
BDEPEND=""

PATCHES=(
	"${FILESDIR}"/moonlight-embedded-rk3588.patch
)

src_prepare() {
	cmake_src_prepare
}

src_configure() {
	cmake_src_configure
}

src_install() {
	cmake_src_install
	mv "${ED}/usr/bin/moonlight" "${ED}/usr/bin/moonlight-embedded"
	mkdir -p "${ED}/etc/moonlight"
	mv "${ED}/usr/etc/moonlight.conf" "${ED}/etc/moonlight/moonlight.conf"
	rmdir "${ED}/usr/etc"
}
