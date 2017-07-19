[![Build Status](https://travis-ci.org/webispy/ocf_mylight.svg?branch=master)](https://travis-ci.org/webispy/ocf_mylight)

# OCF Light sample
- Smart Home Light sample.
- You can control 2 LEDs on the ARTIK 053 STARTER BOARD (TizenRT version only)
  - /a/light/0
  - /a/light/1
- Iotivity secured mode not supported. (TODO)
- This source code was referenced to [TizenRT/apps/examples/iotivity_simpleserver](https://github.com/Samsung/TizenRT/tree/master/apps/examples/iotivity_simpleserver).

# Build
## TizenRT
### Reference document
- https://github.com/samsung/tizenrt

### Build
- Download
```sh
git clone https://github.com/Samsung/TizenRT.git
cd TizenRT
git clone https://github.com/webispy/ocf_mylight apps/examples/ocf_mylight
```
- Configure / Build
```sh
cd TizenRT/os/tools
./configure.sh artik053/iotivity
cd ..
make menuconfig
  Application Configuration
    -> Examples
      -> [ ] Enable building IoTivity Samples
         [*] OCF My Light sample application
make
```

## Linux (Tested Ubuntu 16.04 environment)
### Reference document
- https://wiki.iotivity.org/build_iotivity_with_ubuntu_build_machine

### Build
- Download
```sh
git clone https://github.com/iotivity/iotivity.git -b 1.2-rel
cd iotivity
git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor -b v0.4
git clone https://github.com/webispy/ocf_mylight resource/csdk/stack/samples/linux/ocf_mylight
```

- Modify resource/SConscript
```sh
diff --git a/resource/SConscript b/resource/SConscript
index 249730d..1f6b36c 100644
--- a/resource/SConscript
+++ b/resource/SConscript
@@ -72,6 +72,7 @@ if target_os in ['linux', 'windows']:
        # Build C Samples
        SConscript('csdk/stack/test/linux/SConscript')
        SConscript('csdk/stack/samples/linux/SimpleClientServer/SConscript')
+       SConscript('csdk/stack/samples/linux/ocf_mylight/SConscript')
 
         if env.get('SECURED') == '1':
                # Build secure samples
```

- Build with debug message
```sh
scons -j8 RELEASE=False SECURED=1
```
- Build without debug message
```sh
scons -j8 RELEASE=True LOGGING=False SECURED=1
```

# Run

## TizenRT
```sh
minicom -D /dev/ttyUSB{your-device-serial-number}
TASH> wifi startsta
TASH> wifi scan
TASH> wifi join {SSID} {PASSWORD}
TASH> ifconfig wl1 dhcp
TASH> ocf_mylight
```

## Linux
### Iotivity Debug mode
#### Create a dat file (Convert json to dat using json2cbor tool)
```sh
cd {iotivity-repo}/out/linux/x86_64/debug/resource/csdk/stack/samples/linux/ocf_mylight
../../../../security/tool/json2cbor json/oic_svr_db_server_justworks.json oic_svr_db_server.dat
```

#### Run
```sh
cd {iotivity-repo}/out/linux/x86_64/debug/resource/csdk/stack/samples/linux/ocf_mylight
./ocf_mylight
```

### Iotivity Release mode
#### Create a dat file (Convert json to dat using json2cbor tool)
```sh
cd {iotivity-repo}/out/linux/x86_64/release/resource/csdk/stack/samples/linux/ocf_mylight
../../../../security/tool/json2cbor json/oic_svr_db_server_justworks.json oic_svr_db_server.dat
```

#### Run
```sh
cd {iotivity-repo}/out/linux/x86_64/release/resource/csdk/stack/samples/linux/ocf_mylight
./ocf_mylight
```
