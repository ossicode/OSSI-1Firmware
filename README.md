OSSIREADME 

OSSI-1 인공위성 사용방법

## BEACON 사용법 ##
1. BEACON은 144.25MHZ 에서 12wpm 으로 CW를 신호를 내보낸다. CW는 모스코드로 인코딩 되어 있다
CW 신호의 내용은 'OSSI/1' 와 인공위성 기본정보를 보낸다.

	첫번째(6개 바이트) 인공위성 이름
	
	    OSSI/1 
	
	두번째(5개 바이트) 형식: 1 space errorflag space powerOn
	
		1 0b10100 0b11111 (비트순서:EPS(BATTERY), OBC, BEACON, COMMS, Payload)
	
	세번째(5개 바이트) 형식: 2 space solarvoltage space batteryvoltage
	
		2 0b11111111 0b11111111
	
	각 문장은 Space Space Space 로 구분한다.

2. 신호읽기
OSSI 사이트를 보고 인공위성이 자신의 상공을 지나갈때 무전기를 꺼내어 144.25MHZ를 맞추어 귀로 들으면 된다.

3. 인공위성 찾기
144.25MHZ에서 CW신호에서 나오는 OSSI/1을 찾아 듣는다. 


## COMMS의 동작 ##
지상에서 435.75MHZ로 2FSK로 다음 패킷을 보낸다.

	from ossi import Uplink, Downlink, Request
	
	u = Uplink(435.75, '2FSK', 1200)
 	d = Downlink(435.75, '2FSK', 9600)
	request = Request(u, d)

	# EPS
	## 온도 
	temp = request.getBatteryTemperature()
	temp = request.getPannelTemperature()
	x1_temp = request.getSolarPannelTemperature(X,1)
	z1_temp = request.getSolarPannelTemperature(Z,1)
	y1_temp = request.getSolarPannelTemperature(Y,1)

	## volatge
	v = request.getBatteryVoltage()
	v = request.getSolarPannelVoltage()

	## solar sensor
	s = request.getSolarSensor()

	## 충전 정보
	check = request.checkBatteryCharge()
	

	## 솔라, 배터리, SC 켜져 있는가?
	check = request.checkSolarOn()
	check = request.checkBatteryOn()
	check = request.checkSuperCapOn()

	## 수퍼캡 
	status = request.doSuperCapOn()
	check = request.checkSuperCapCharge()

	## EPS reset!
	request.resetEPS()

	# OBC
	## OBC reset!
	request.resetOBC()
	
	## 자이로
	l = request.getGyro()

	## RTC 시간 가져오기 
	time = request.getRtcTime()
	status = request.setRtcTime(datetime.datetime.now())

	## Payload LED
	reserveStatus = request.reserveMessage("OSSI 1", datetime.datetime(2012, 12, 12, 12, 12))
	reserveList = request.getReserveList()

	request.ledOn()
	request.ledOFF()
	
	## 안테나 펴졌는지?
	check = request.checkAntDeploy()
	## 안테나 펴기
	request.antDeploy()

	request.resetPayload()

	# COMMS
	request.resetCOMMS()

	# BEACON
	request.resetBEACON()

	# STORAGE(EEPROM, FRAM)
	status = request.setByteEEPROM(1, 'x')
	
http://swisscube-live.ch/Publish/S3-D-ICD-1-0a-Housekeeping_ICD.pdf



### 패킷 포멧 ###

예제 TX 패킷

OxAA, 0xAA, 0xAA, 0xAA, 0xD3, 0x91, 0xAA, 0xAA, 0xAA, '1', '2', '3', '4', '5', '6', 0xAA, 0xAA, 0XAA

p1, p2, p3, p4, s1, s2, len, command1, c2, c3, d1~dn, crc

#### ax.25 ####
http://swisscube-live.ch/Publish/S3-BC-SE-1-1b-AX.25%20Transfer%20Frames%20Format.pdf

![](http://i.imgur.com/MygEt.png)

- 01111110 # flag
- callsign 48bits # destination
- ssid 8bit 011ssid0
- callsign 48bits # source
- ssid 8bit 011ssid0
- control bits 00000011
- protocal identifier 0xF0
- Information field(32-2048)
- Frame-Check Sequence
- 01111110 # flag

**Information field**

![](http://i.imgur.com/Xdnas.png)

- version number: 00
- virtual channel Id: 000
- spare: 000
- master frame count 0x00
- virtual channel frame count 0x00
- first header pointer 0x00
- data 
- time flag 0b000
- spare 00
- tc count 00
- time 

## UART로 통신하기 ##
OBC에 있는 인터페이스를 통해서 다른 모듈을 제어 할 수 있다. 

인터페이스는 동일 

	from ossi import Serial Request
	
 	s = Serial('Com3', 9600)
	request = Request(s)



## 인공위성 dashboard ##

![](http://www.devco.net/images/gdash-small.png)

https://github.com/ripienaar/graphite-graph-dsl/wiki
