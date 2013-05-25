<?xml version="1.0" encoding="UTF-8"?>

<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:util="http://www.springframework.org/schema/util"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
                           http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-2.5.xsd">

    <!--                                    -->
    <!-- The prodb_mopstats data source -->
    <!--                                    -->

    <bean id="mopStatsDataSource1" class="org.apache.commons.dbcp.BasicDataSource">
        <property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
        <property name="url"
                  value="jdbc:mysql://${mopstatsdatasource1.ipport}/prodb_service?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
        <property name="username" value="${mopstatsdatasource1.username}"></property>
        <property name="password" value="${mopstatsdatasource1.password}"></property>
        <property name="maxActive" value="${mopstatsdatasource1.maxactive}"></property>
        <property name="maxIdle" value="${mopstatsdatasource1.maxidle}"></property>
        <property name="maxWait" value="${mopstatsdatasource1.maxwait}"></property>
        <property name="defaultAutoCommit" value="true"></property>
        <property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
    </bean>

    <bean id="mopStatsDataSource2" class="org.apache.commons.dbcp.BasicDataSource">
        <property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
        <property name="url"
                  value="jdbc:mysql://${mopstatsdatasource2.ipport}/prodb_service?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
        <property name="username" value="${mopstatsdatasource2.username}"></property>
        <property name="password" value="${mopstatsdatasource2.password}"></property>
        <property name="maxActive" value="${mopstatsdatasource2.maxactive}"></property>
        <property name="maxIdle" value="${mopstatsdatasource2.maxidle}"></property>
        <property name="maxWait" value="${mopstatsdatasource2.maxwait}"></property>
        <property name="defaultAutoCommit" value="true"></property>
        <property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
    </bean>

    <util:map id="mopStatsDataSources">
        <entry key="psds1" value-ref="mopStatsDataSource1"/>
        <entry key="psds2" value-ref="mopStatsDataSource2"/>
    </util:map>

    <bean id="mopStatsDataSourceLookup"
          class="org.springframework.jdbc.datasource.lookup.MapDataSourceLookup">
        <constructor-arg>
            <ref bean="mopStatsDataSources"/>
        </constructor-arg>
    </bean>

    <bean id="mopStatsDataSource" class="com.baidu.pic.base.datasource.AutoDataSource">
        <property name="defaultTargetDataSource" ref="mopStatsDataSource1"/>
        <property name="targetDataSources" ref="mopStatsDataSources"/>
        <property name="dataSourceLookup" ref="mopStatsDataSourceLookup"/>
    </bean>

    <bean id="mopStatsTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource">
            <ref bean="mopStatsDataSource"/>
        </property>
    </bean>

    <bean id="mopStatsTransactionTemplate" class="org.springframework.transaction.support.TransactionTemplate">
        <property name="transactionManager">
            <ref bean="mopStatsTransactionManager"/>
        </property>
    </bean>

    <bean id="mopStatsJT" class="org.springframework.jdbc.core.JdbcTemplate">
        <property name="dataSource" ref="mopStatsDataSource"/>
    </bean>

</beans>
