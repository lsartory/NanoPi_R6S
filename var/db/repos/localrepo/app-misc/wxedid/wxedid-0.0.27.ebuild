# Copyright 1999-2021 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=7

WX_GTK_VER="3.0-gtk3"
inherit wxwidgets

DESCRIPTION="Extended Display Identification Data editor"
HOMEPAGE="https://sourceforge.net/projects/wxedid/"
SRC_URI="mirror://sourceforge/wxedid/${P}.tar.gz"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="arm64"

RDEPEND="x11-libs/wxGTK:${WX_GTK_VER}[X]"
DEPEND="${RDEPEND}"

src_prepare() {
	default
}

src_configure() {
	setup-wxwidgets
	econf
}

src_install() {
	default
}
