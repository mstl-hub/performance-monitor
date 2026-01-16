package device

type DeviceTheme int

const (
	Default DeviceTheme = iota
)

type Device struct {
	IsConnected bool
	Themes []string
}

func (d *Device) AttempConnection() bool {
	return false
}