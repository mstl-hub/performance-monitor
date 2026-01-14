package device

type Device struct {
	IsConnected bool
}

func (d *Device) AttempConnection() bool {
	return false
}