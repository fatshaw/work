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
    <!-- The Mongo					-->
    <!--                            -->

	<bean id="dmqServer" class="com.mongodb.Mongo">

		<constructor-arg type="java.lang.String">
			<value>${dmqserver.ip}</value>
		</constructor-arg>

		<constructor-arg type="int">
			<value>${dmqserver.port}</value>
		</constructor-arg>
	
	</bean>

    <!--                            -->
    <!-- The dmqSeaCacheDB			-->
    <!--                            -->

	<bean id="dmqSeaCacheDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="dmqServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqseacachedbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqseacachedbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqseacachedbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${dmqseacachedbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="dmqSeaCacheDB" factory-bean="dmqSeaCacheDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The dmqInfoDB				-->
    <!--                            -->

	<bean id="dmqInfoDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="dmqServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqinfodbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqinfodbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqinfodbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${dmqInfoDBProxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="dmqInfoDB" factory-bean="dmqInfoDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The dmqAdminDB		-->
    <!--                            -->

	<bean id="dmqAdminDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="dmqServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqadmindbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqadmindbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${dmqadmindbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${dmqadmindbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="dmqAdminDB" factory-bean="dmqAdminDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- various DBCollection		-->
    <!--                            -->
	<bean id="dmqConfigInfoName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${dmqconfiginfodbname}</value>
		</constructor-arg>
	</bean>
	<bean id="dmqClientInfoName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${dmqclientinfodbname}</value>
		</constructor-arg>
	</bean>
	<bean id="dmqConfigInfo" factory-bean="dmqInfoDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="dmqConfigInfoName" />
		</constructor-arg>
    </bean>
	<bean id="dmqClientInfo" factory-bean="dmqInfoDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="dmqClientInfoName" />
		</constructor-arg>
    </bean>

</beans>
