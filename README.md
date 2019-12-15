# libzbxrdkafka #

> A Zabbix loadable module to export history data into a Kafka topic

## Configuration ##

The configuration file accepts the following parameters:

    # Set this to the list of Kafka Brokers to use as Bootstrap Servers.
    KafkaBootstrapServers=broker1:9200,broker2:9200
    
    # This is the topic that Zabbix History will be sent to. The schema will be fixed.
    KafkaTopic=zabbix-history

### Credits ###

With apologies to LMacPhail, using his module 
[zabbix-history-influxdb](https://github.com/LMacPhail/zabbix-history-influxdb) as the basis for structure.
