package themes

import (
	"image/color"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/theme"
)

const (
	ColorText       fyne.ThemeColorName = theme.ColorNameForeground
	ColorBackGround                     = theme.ColorNameBackground
	ColorError                          = theme.ColorNameError
	ColorSuccess                        = theme.ColorNameSuccess
	ColorButton                         = theme.ColorNameButton
)

func newColorsMap() map[fyne.ThemeColorName]color.Color {
	m := make(map[fyne.ThemeColorName]color.Color)
	m[ColorText] = color.RGBA{245, 245, 245, 255}
	m[ColorBackGround] = color.RGBA{29, 29, 29, 255}
	m[ColorError] = color.RGBA{124, 33, 41, 255}
	m[ColorSuccess] = color.RGBA{59, 124, 33, 255}
	m[ColorButton] = color.RGBA{31, 45, 90, 255}

	return m
}

const (
	IconPerformanceMonitor fyne.ThemeIconName = "IconPerformanceMonitor"
	IconShieldCheck        fyne.ThemeIconName = "IconShieldCheck"
	IconShieldX            fyne.ThemeIconName = "IconShieldX"
)

func newIconsMap() map[fyne.ThemeIconName]fyne.Resource {
	m := make(map[fyne.ThemeIconName]fyne.Resource)
	m[IconPerformanceMonitor] = resourcePerformanceMonitorIco
	m[IconShieldCheck] = resourceShieldCheckIco
	m[IconShieldX] = resourceShieldXIco

	return m
}

const ()

func newFontsMap() map[fyne.TextStyle]fyne.Resource {
	m := make(map[fyne.TextStyle]fyne.Resource)

	return m
}

const (
	H1 fyne.ThemeSizeName = "H1"
	H2 fyne.ThemeSizeName = "H2"
	H3 fyne.ThemeSizeName = "H3"
	H4 fyne.ThemeSizeName = "H4"
	H5 fyne.ThemeSizeName = "H5"
)

func newSizesMap() map[fyne.ThemeSizeName]float32 {
	m := make(map[fyne.ThemeSizeName]float32)
	baseSize := theme.TextSize()
	m[H1] = baseSize * 3.75
	m[H2] = baseSize * 3
	m[H3] = baseSize * 2
	m[H4] = baseSize * 1.5
	m[H5] = baseSize * 1.25

	return m
}

type AppTheme struct {
	Colors map[fyne.ThemeColorName]color.Color
	Icons  map[fyne.ThemeIconName]fyne.Resource
	Fonts  map[fyne.TextStyle]fyne.Resource
	Sizes  map[fyne.ThemeSizeName]float32
}

func NewAppTheme() *AppTheme {
	appTheme := &AppTheme{}
	appTheme.Colors = newColorsMap()
	appTheme.Icons = newIconsMap()
	appTheme.Fonts = newFontsMap()
	appTheme.Sizes = newSizesMap()

	return appTheme
}

func (appTheme AppTheme) Color(name fyne.ThemeColorName, variant fyne.ThemeVariant) color.Color {
	res, ok := appTheme.Colors[name]
	if ok {
		return res
	}

	return theme.DefaultTheme().Color(name, 0)
}

func (appTheme AppTheme) Icon(name fyne.ThemeIconName) fyne.Resource {
	res, ok := appTheme.Icons[name]
	if ok {
		return res
	}

	return theme.DefaultTheme().Icon(name)
}

func (appTheme AppTheme) Font(style fyne.TextStyle) fyne.Resource {
	res, ok := appTheme.Fonts[style]
	if ok {
		return res
	}

	return theme.DefaultTheme().Font(style)
}

func (appTheme AppTheme) Size(name fyne.ThemeSizeName) float32 {
	res, ok := appTheme.Sizes[name]
	if ok {
		return res
	}

	return theme.DefaultTheme().Size(name)
}
