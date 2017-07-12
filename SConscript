#******************************************************************
#
# Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

thread_env = SConscript('#build_common/thread.scons')
samples_env = thread_env.Clone()
target_os = samples_env.get('TARGET_OS')
with_ra = samples_env.get ('WITH_RA')

######################################################################
# Build flags
######################################################################
with_upstream_libcoap = samples_env.get('WITH_UPSTREAM_LIBCOAP')
if with_upstream_libcoap == '1':
	# For bring up purposes only, we manually copy the forked version to where the unforked version is downloaded.
	samples_env.AppendUnique(CPPPATH = ['#extlibs/libcoap/libcoap/include'])
else:
	# For bring up purposes only, the forked version will live here.
	samples_env.AppendUnique(CPPPATH = ['../../../../../connectivity/lib/libcoap-4.1.1/include'])

samples_env.PrependUnique(CPPPATH = [
		'../../../../logger/include',
		'../../../../stack/include',
		'../../../../security/include',
		'../../../../../../extlibs/cjson',
		'../../../../../../extlibs/boost/boost',
		'../../../../../oc_logger/include',
		])

compiler = samples_env.get('CXX')
if 'g++' in compiler:
	samples_env.AppendUnique(CXXFLAGS = ['-std=c++0x', '-Wall'])

samples_env.AppendUnique(RPATH = [samples_env.get('BUILD_DIR')])
samples_env.AppendUnique(LIBPATH = [samples_env.get('BUILD_DIR')])

samples_env.PrependUnique(LIBS = ['octbstack', 'ocsrm', 'connectivity_abstraction', 'coap'])

if target_os not in ['arduino', 'windows', 'darwin', 'ios', 'msys_nt']:
	samples_env.AppendUnique(LIBS = ['rt'])

if target_os not in ['windows']:
	samples_env.PrependUnique(LIBS = ['m'])

samples_env.AppendUnique(CPPDEFINES = ['TB_LOG'])

######################################################################
# Source files and Targets
######################################################################
ocserver         = samples_env.Program('ocf_mylight', [
			'ocf_mylight_main.c',
			'ocf_mylight_device.c',
			'ocf_mylight_light.c',
			'ocf_mylight_verbose.c',
			'ocf_mylight_configuration.c',
			'ocf_mylight_maintenance.c',
			'ocf_mylight_platform.c'
			])

list_of_samples = [ocserver,]
Alias("samples", list_of_samples)

samples_env.AppendTarget('samples')


