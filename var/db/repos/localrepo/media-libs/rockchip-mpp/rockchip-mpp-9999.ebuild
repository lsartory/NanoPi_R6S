# Copyright 1999-2020 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=7
inherit cmake flag-o-matic

inherit git-r3
EGIT_REPO_URI="https://github.com/rockchip-linux/mpp.git"
#EGIT_REPO_URI="https://github.com/JeffyCN/rockchip_mirrors.git"
#EGIT_BRANCH="mpp"
SRC_URI=""

KEYWORDS="arm64"

DESCRIPTION="Rockchip Media Process Platform (MPP)"
HOMEPAGE="https://github.com/rockchip-linux/mpp"

SLOT="0"

DEPEND=""
RDEPEND=""

src_prepare() {
	cmake_src_prepare
}

src_configure() {
	append-ldflags $(no-as-needed)

	mycmakeargs=(
		-DRKPLATFORM=ON
		-DHAVE_DRM=ON
	)

	cmake_src_configure
}

src_install() {
	cmake_src_install
}
