**Definitions**
WTS – wireless temperature sensor. It sends data by LoRa.

**List fo commands**

<table>
	<tr>
		<th>Prefix</th>
		<th>Code</th>
		<th>Length</th>
		<th>Name of command</th>
		<th>Flags</th>
		<th>Response</th>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>40h</td>
		<td>3</td>
		<td>StartUpdating</td>
		<td>(0, 0)</td>
		<td>command status</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>41h</td>
		<td>3</td>
		<td>EraseMainApp</td>
		<td>(0, 0)</td>
		<td>command status</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>42h</td>
		<td>133</td>
		<td>ProgrammData</td>
		<td>(0, 0)</td>
		<td>command status</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>43h</td>
		<td>3</td>
		<td>StopUpdating</td>
		<td>(0, 0)</td>
		<td>command status</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>44h</td>
		<td>4</td>
		<td>CheckAppCRC</td>
		<td>(0, 0)</td>
		<td>bool</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>45h</td>
		<td>3</td>
		<td>CheckAppExistence</td>
		<td>(0, 0)</td>
		<td>bool</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>46h</td>
		<td>3</td>
		<td>IsMemoryProtect</td>
		<td>(0, 0)</td>
		<td>bool</td>
	</tr>
	<tr>
		<td>1Ah</td>
		<td>47h</td>
		<td>4</td>
		<td>SetMemoryProtection</td>
		<td>(0, 0)</td>
		<td>command status</td>
	</tr>
</table>
<br/>
<br/>

**Command Descriptions**

Below is a detailed description of each command and its behavior.

*StartUpdating*

    Code: 0x40
    Description: Initializes firmware update procedure. Prepares internal state to accept firmware data blocks.
    Payload: None
    Response: command status – always returns SUCCESS as confirmation

*EraseMainApp*

    Code: 0x41
    Description: Erases the existing main application firmware.
    Payload: None
    Response: command status – result of erase operation (SUCCESS if success)

*ProgrammData*

    Code: 0x42
    Description: Sends an encrypted block of firmware data to be decrypted and programmed into flash.
    Payload: 130 bytes (first 2 bytes is a block number, next 128 bytes contain encrypted firmware block)
    Response: command status – result of program operation (SUCCESS if success)

*StopUpdating*

    Code: 0x43
    Description: Ends the firmware update session and performs finalization steps.
    Payload: None
    Response: command status – always returns SUCCESS as confirmation

*CheckAppCRC*

    Code: 0x44
    Description: Verifies the CRC of the programmed application using the provided CRC value.
    Payload: 1 byte – expected CRC value
    Response: bool – result of comparison

*CheckAppExistence*

    Code: 0x45
    Description: Checks whether a valid application exists in flash.
    Payload: None
    Response: bool – true if a valid application is detected

*IsMemoryProtect*

    Code: 0x46
    Description: Checks whether memory protection is enabled.
    Payload: None
    Response: bool – true if protection is enabled

*SetMemoryProtection*

    Code: 0x47
    Description: Enables or disables memory protection.
    Payload: 1 byte – bool indicating desired protection state
    Response: command status – SUCCESS if the protection state was successfully changed

<br/><br/>

### **Binary Protocol Examples**

All messages follow this structure:

```
[Prefix][Code][Payload...][CRC]
```

Response format:

```
[Prefix][Code][Response byte 1][Response byte 2]...[Response byte N][CRC]
```

#### 📤 StartUpdating (0x40)

**Request**:

```
1A 40 [CRC]
```

**Response**:

```
5B 01 [CRC]
```

---

#### 📤 EraseMainApp (0x41)

**Request**:

```
1A 41 [CRC]
```

**Response (Success)**:

```
5B 01 [CRC]
```

**Response (Failure)**:

```
5B 00 [CRC]
```

---

#### 📤 ProgrammData (0x42)

**Request (example)**:

```
1A 42 <2-byte block number> <128-byte encrypted data block> [CRC]
```

**Response**:

```
5B 01 [CRC]
```

---

#### 📤 StopUpdating (0x43)

**Request**:

```
1A 43 [CRC]
```

**Response**:

```
5B 01 [CRC]
```

---

#### 📤 CheckAppCRC (0x44)

**Request**:

```
1A 44 <expected CRC byte> [CRC]
```

**Response**:

```
5B 01 [CRC] // CRC matches  
5B 00 [CRC] // CRC mismatch
```

---

#### 📤 CheckAppExistence (0x45)

**Request**:

```
1A 45 [CRC]
```

**Response**:

```
5B 01 [CRC]  // App exists  
5B 00 [CRC]  // App not found
```

---

#### 📤 IsMemoryProtect (0x46)

**Request**:

```
1A 46 [CRC]
```

**Response**:

```
5B 01 [CRC]  // Protection enabled  
5B 00 [CRC]  // Protection disabled
```

---

#### 📤 SetMemoryProtection (0x47)

**Request (enable protection)**:

```
1A 47 01 [CRC]
```

**Response**:

```
5B 01 [CRC]
```

**Request (disable protection)**:

```
1A 47 00 [CRC]
```

**Response**:

```
5B 01 [CRC]
```

---







