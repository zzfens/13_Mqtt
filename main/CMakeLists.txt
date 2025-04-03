idf_component_register(SRCS "app_main.c" "./wifi/wifito.c" "./I2S/I2Sto.c" "./mqtt/mqttto.c" "./led/ledto.c"
                       INCLUDE_DIRS "." "./wifi" "./I2S" "./mqtt" "./led")
