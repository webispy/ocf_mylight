#******************************************************************
#
# Copyright: 2016, Samsung Electronics Co., Ltd.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

Import('stacksamples_env')
mysample_env = stacksamples_env.Clone()

target_os = stacksamples_env.get('TARGET_OS')

######################################################################
# Build flags
######################################################################
mysample_env.PrependUnique(CPPPATH=[
    '#/resource/csdk/include',
    '#/resource/csdk/stack/include',
    '#/resource/csdk/security/include',
    '#/resource/csdk/connectivity/api',
])

if target_os not in ['msys_nt', 'windows']:
    mysample_env.PrependUnique(LIBS=['connectivity_abstraction',])

mysample_env.PrependUnique(LIBS=['octbstack',])

if mysample_env.get('SECURED') == '1':
    if mysample_env.get('WITH_TCP') == True:
        mysample_env.AppendUnique(LIBS=['mbedtls', 'mbedx509', 'mbedcrypto'])

mysample_env.AppendUnique(LIBPATH=[mysample_env.get('BUILD_DIR'),])

######################################################################
# Source files and Targets
######################################################################
ocserver = mysample_env.Program('ocf_mylight', [
			'ocf_mylight_main.c',
			'ocf_mylight_device.c',
			'ocf_mylight_light.c',
			'ocf_mylight_verbose.c',
			'ocf_mylight_configuration.c',
			'ocf_mylight_maintenance.c',
			'ocf_mylight_platform.c',
			'ocf_mylight_security.c',
			'ocf_mylight_userinput.c',
			'ocf_mylight_notify.c'
			])
