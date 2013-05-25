<?xml version="1.0" encoding="UTF-8"?>

<beans xmlns="http://www.springframework.org/schema/beans"
	   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	   xmlns:aop="http://www.springframework.org/schema/aop"
	   xmlns:context="http://www.springframework.org/schema/context" 
	   xmlns:tx="http://www.springframework.org/schema/tx"
	   xmlns:p="http://www.springframework.org/schema/p"
	   xmlns:util="http://www.springframework.org/schema/util"
	   xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
                           http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
                           http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd
                           http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd
                           http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-2.5.xsd">

    <!--                            -->
    <!-- The prodb_mgmt_storage data source -->
    <!--                            -->
    
    <bean id="prodbMgmtDataSource1" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url"
 			value="jdbc:mysql://${prodbmgmtdatasource1.ipport}/prodb_mgmt?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
		<property name="username" value="${prodbmgmtdatasource1.username}"></property>
		<property name="password" value="${prodbmgmtdatasource1.password}"></property>
		<property name="maxActive" value="${prodbmgmtdatasource1.maxactive}"></property>
		<property name="maxIdle" value="${prodbmgmtdatasource1.maxidle}"></property>
		<property name="maxWait" value="${prodbmgmtdatasource1.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
	</bean>
	
	<bean id="prodbMgmtDataSource2" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url"
 			value="jdbc:mysql://${prodbmgmtdatasource2.ipport}/prodb_mgmt?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
 		<property name="username" value="${prodbmgmtdatasource2.username}"></property>
		<property name="password" value="${prodbmgmtdatasource2.password}"></property>
		<property name="maxActive" value="${prodbmgmtdatasource2.maxactive}"></property>
		<property name="maxIdle" value="${prodbmgmtdatasource2.maxidle}"></property>
		<property name="maxWait" value="${prodbmgmtdatasource2.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
	</bean>
	
	<util:map id="prodbMgmtDataSources">
        <entry key="pmds1" value-ref="prodbMgmtDataSource1" />
        <entry key="pmds2" value-ref="prodbMgmtDataSource2" />
    </util:map>
    
    <bean id="prodbMgmtDataSourceLookup"
         class="org.springframework.jdbc.datasource.lookup.MapDataSourceLookup">
         <constructor-arg>
             <ref bean="prodbMgmtDataSources" />
         </constructor-arg>
    </bean>
    
    <bean id="prodbMgmtDataSource" class="com.baidu.pic.base.datasource.AutoDataSource">
        <property name="defaultTargetDataSource" ref="prodbMgmtDataSource1" />
        <property name="targetDataSources" ref="prodbMgmtDataSources" />
        <property name="dataSourceLookup" ref="prodbMgmtDataSourceLookup" />
    </bean>

    <bean id="prodbMgmtTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource">
			<ref bean="prodbMgmtDataSource"/>
		</property> 
	</bean>
    
    <bean id="prodbMgmtTransactionTemplate" class="org.springframework.transaction.support.TransactionTemplate">
		<property name="transactionManager">
			<ref bean="prodbMgmtTransactionManager"/>
		</property> 
	</bean>
    
    <bean id="prodbMgmtjt" class="org.springframework.jdbc.core.JdbcTemplate">
		<property name="dataSource" ref="prodbMgmtDataSource" />
	</bean>
	
	<!--                                    -->
    <!-- The prodb_feed_testing data source -->
    <!--                                    -->
	
    <bean id="prodbFeedTestingDataSource1" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url"
 			value="jdbc:mysql://${prodbfeedtestingdatasource1.ipport}/prodb_feed_testing?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
		<property name="username" value="${prodbfeedtestingdatasource1.username}"></property>
		<property name="password" value="${prodbfeedtestingdatasource1.password}"></property>
		<property name="maxActive" value="${prodbfeedtestingdatasource1.maxactive}"></property>
		<property name="maxIdle" value="${prodbfeedtestingdatasource1.maxidle}"></property>
		<property name="maxWait" value="${prodbfeedtestingdatasource1.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
	</bean>
	
	<bean id="prodbFeedTestingDataSource2" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url"
 			value="jdbc:mysql://${prodbfeedtestingdatasource2.ipport}/prodb_feed_testing?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
		<property name="username" value="${prodbfeedtestingdatasource2.username}"></property>
		<property name="password" value="${prodbfeedtestingdatasource2.password}"></property>
		<property name="maxActive" value="${prodbfeedtestingdatasource1.maxactive}"></property>
		<property name="maxIdle" value="${prodbfeedtestingdatasource1.maxidle}"></property>
		<property name="maxWait" value="${prodbfeedtestingdatasource1.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
	</bean>
	
	<util:map id="prodbFeedTestingDataSources">
        <entry key="pftds1" value-ref="prodbFeedTestingDataSource1" />
        <entry key="pftds2" value-ref="prodbFeedTestingDataSource2" />
    </util:map>
    
    <bean id="prodbFeedTestingDataSourceLookup"
         class="org.springframework.jdbc.datasource.lookup.MapDataSourceLookup">
         <constructor-arg>
             <ref bean="prodbFeedTestingDataSources" />
         </constructor-arg>
    </bean>
    
    <bean id="prodbFeedTestingDataSource" class="com.baidu.pic.base.datasource.AutoDataSource">
        <property name="defaultTargetDataSource" ref="prodbFeedTestingDataSource1" />
        <property name="targetDataSources" ref="prodbFeedTestingDataSources" />
        <property name="dataSourceLookup" ref="prodbFeedTestingDataSourceLookup" />
    </bean>
    
    <bean id="prodbFeedTestingTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource">
			<ref bean="prodbFeedTestingDataSource"/>
		</property> 
	</bean>
    
    <bean id="prodbFeedTestingTransactionTemplate" class="org.springframework.transaction.support.TransactionTemplate">
		<property name="transactionManager">
			<ref bean="prodbFeedTestingTransactionManager"/>
		</property> 
	</bean>
    
    <bean id="prodbFeedTestingjt" class="org.springframework.jdbc.core.JdbcTemplate">
		<property name="dataSource" ref="prodbFeedTestingDataSource" />
	</bean>
	
	
    <!--                                    -->
    <!-- The prodb_product_info data source -->
    <!--                                    -->
	
	<bean id="prodbProductInfoDataSource1" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url" 
 			value="jdbc:mysql://${prodbproductinfodatasource1.ipport}/prodb_product_info?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property> 
		<property name="username" value="${prodbproductinfodatasource1.username}"></property>
		<property name="password" value="${prodbproductinfodatasource1.password}"></property>
		<property name="maxActive" value="${prodbproductinfodatasource1.maxactive}"></property>
		<property name="maxIdle" value="${prodbproductinfodatasource1.maxidle}"></property>
		<property name="maxWait" value="${prodbproductinfodatasource1.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
        <property name="numTestsPerEvictionRun" value="20"></property>
        <property name="timeBetweenEvictionRunsMillis" value="60000"></property>
	</bean>
	
	<bean id="prodbProductInfoDataSource2" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url" 
 			value="jdbc:mysql://${prodbproductinfodatasource2.ipport}/prodb_product_info?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property> 
		<property name="username" value="${prodbproductinfodatasource2.username}"></property>
		<property name="password" value="${prodbproductinfodatasource2.password}"></property>
		<property name="maxActive" value="${prodbproductinfodatasource2.maxactive}"></property>
		<property name="maxIdle" value="${prodbproductinfodatasource2.maxidle}"></property>
		<property name="maxWait" value="${prodbproductinfodatasource2.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
        <property name="numTestsPerEvictionRun" value="20"></property>
        <property name="timeBetweenEvictionRunsMillis" value="60000"></property>
        
	</bean>
	
	<util:map id="prodbProductInfoDataSources">
        <entry key="ppids1" value-ref="prodbProductInfoDataSource1" />
        <entry key="ppids2" value-ref="prodbProductInfoDataSource2" />
    </util:map>
    
    <bean id="prodbProductInfoDataSourceLookup"
         class="org.springframework.jdbc.datasource.lookup.MapDataSourceLookup">
         <constructor-arg>
             <ref bean="prodbProductInfoDataSources" />
         </constructor-arg>
    </bean>
    
    <bean id="prodbProductInfoDataSource" class="com.baidu.pic.base.datasource.AutoDataSource">
        <property name="defaultTargetDataSource" ref="prodbProductInfoDataSource1" />
        <property name="targetDataSources" ref="prodbProductInfoDataSources" />
        <property name="dataSourceLookup" ref="prodbProductInfoDataSourceLookup" />
    </bean>

    <bean id="prodbProductInfoTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource">
			<ref bean="prodbProductInfoDataSource"/>
		</property> 
	</bean>
    
    <bean id="prodbProductInfoTransactionTemplate" class="org.springframework.transaction.support.TransactionTemplate">
		<property name="transactionManager">
			<ref bean="prodbProductInfoTransactionManager"/>
		</property> 
	</bean>
    
    <bean id="prodbProductInfojt" class="org.springframework.jdbc.core.JdbcTemplate">
		<property name="dataSource" ref="prodbProductInfoDataSource" />
	</bean>
	
	<!--                                    -->
    <!-- The prodb_service data source -->
    <!--                                    -->
    
	<bean id="prodbServiceDataSource1" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url"
 			value="jdbc:mysql://${prodbservicedatasource1.ipport}/prodb_service?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
		<property name="username" value="${prodbservicedatasource1.username}"></property>
		<property name="password" value="${prodbservicedatasource1.password}"></property>
		<property name="maxActive" value="${prodbservicedatasource1.maxactive}"></property>
		<property name="maxIdle" value="${prodbservicedatasource1.maxidle}"></property>
		<property name="maxWait" value="${prodbservicedatasource1.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
	</bean>
	
	<bean id="prodbServiceDataSource2" class="org.apache.commons.dbcp.BasicDataSource">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
 		<property name="url"
 			value="jdbc:mysql://${prodbServiceDataSource1.ipport}/prodb_service?useUnicode=true&amp;characterEncoding=UTF-8&amp;zeroDateTimeBehavior=convertToNull"></property>
 		<property name="username" value="${prodbServiceDataSource1.username}"></property>
		<property name="password" value="${prodbServiceDataSource1.password}"></property>
		<property name="maxActive" value="${prodbServiceDataSource1.maxactive}"></property>
		<property name="maxIdle" value="${prodbServiceDataSource1.maxidle}"></property>
		<property name="maxWait" value="${prodbServiceDataSource1.maxwait}"></property>
		<property name="defaultAutoCommit" value="true"></property>
		<property name="validationQuery" value="select 1"></property>
        <property name="testOnBorrow" value="true"></property>
        <property name="testOnReturn" value="true"></property>
        <property name="testWhileIdle" value="true"></property>
	</bean>
	
	<util:map id="prodbServiceDataSources">
        <entry key="psds1" value-ref="prodbServiceDataSource1" />
        <entry key="psds2" value-ref="prodbServiceDataSource2" />
    </util:map>
    
    <bean id="prodbServiceDataSourceLookup"
         class="org.springframework.jdbc.datasource.lookup.MapDataSourceLookup">
         <constructor-arg>
             <ref bean="prodbServiceDataSources" />
         </constructor-arg>
    </bean>
    
    <bean id="prodbServiceDataSource" class="com.baidu.pic.base.datasource.AutoDataSource">
        <property name="defaultTargetDataSource" ref="prodbServiceDataSource1" />
        <property name="targetDataSources" ref="prodbServiceDataSources" />
        <property name="dataSourceLookup" ref="prodbServiceDataSourceLookup" />
    </bean>

    <bean id="prodbServiceTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource">
			<ref bean="prodbServiceDataSource"/>
		</property> 
	</bean>
    
    <bean id="prodbServiceTransactionTemplate" class="org.springframework.transaction.support.TransactionTemplate">
		<property name="transactionManager">
			<ref bean="prodbServiceTransactionManager"/>
		</property> 
	</bean>
    
    <bean id="prodbServicejt" class="org.springframework.jdbc.core.JdbcTemplate">
		<property name="dataSource" ref="prodbServiceDataSource" />
	</bean>
	
</beans>