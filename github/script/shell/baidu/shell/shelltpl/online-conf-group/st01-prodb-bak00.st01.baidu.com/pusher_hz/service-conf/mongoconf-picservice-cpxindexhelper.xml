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

	<bean id="indexHelperServer" class="com.mongodb.Mongo">

		<constructor-arg type="java.lang.String">
			<value>${indexHelperServer.ip}</value>
		</constructor-arg>

		<constructor-arg type="int">
			<value>${indexhelperServer.port}</value>
		</constructor-arg>
	
	</bean>

    <!--                            -->
    <!-- The indexHelperDBProxy.dbname			-->
    <!--                            -->

	<bean id="indexHelperDBProxy.dbnameProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="indexHelperServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperDBProxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperDBProxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperDBProxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${indexHelperDBProxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="indexHelperDBProxy.dbname" factory-bean="indexHelperDBProxy.dbnameProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The indexHelperTestDBProxy.dbname		-->
    <!--                            -->

	<bean id="indexHelperTestDBProxy.dbnameProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="indexHelperServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperTestDBProxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperTestDBProxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperTestDBProxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${indexHelperTestDBProxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="indexHelperTestDBProxy.dbname" factory-bean="indexHelperTestDBProxy.dbnameProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The indexHelperAdminDB		-->
    <!--                            -->

	<bean id="indexHelperAdminDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="indexHelperServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperAdminDBProxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperAdminDBProxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${indexHelperAdminDBProxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${indexHelperAdminDBProxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="indexHelperAdminDB" factory-bean="indexHelperAdminDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- various DBCollection		-->
    <!--                            -->

	<bean id="cpxIndexHelperName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>cpxIndexHelper</value>
		</constructor-arg>
	</bean>
	<bean id="cpxIndexHelperTestName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>cpxIndexHelperT</value>
		</constructor-arg>
	</bean>
	<bean id="cpxIndexHelper" factory-bean="indexHelperDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
			<ref bean="cpxIndexHelperName" />
		</constructor-arg>
	</bean>
	<bean id="cpxIndexHelperT" factory-bean="indexHelperTestDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
			<ref bean="cpxIndexHelperTestName" />
		</constructor-arg>
	</bean>

</beans>
