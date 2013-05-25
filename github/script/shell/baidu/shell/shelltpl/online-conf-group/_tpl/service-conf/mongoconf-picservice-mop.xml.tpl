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

	<bean id="mopServer" class="com.mongodb.Mongo">

		<constructor-arg type="java.lang.String">
			<value>${mopserver.ip}</value>
		</constructor-arg>

		<constructor-arg type="int">
			<value>${mopserver.port}</value>
		</constructor-arg>
	
	</bean>

    <!--                            -->
    <!-- The mopMORepoDB		-->
    <!--                            -->

	<bean id="mopMORepoDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="mopServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopmorepodbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopmorepodbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopmorepodbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${mopmorepodbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="mopMORepoDB" factory-bean="mopMORepoDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The mopPDRepoDB	-->
    <!--                            -->

	<bean id="mopPDRepoDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="mopServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${moppdrepodbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${moppdrepodbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${moppdrepodbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${moppdrepodbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="mopPDRepoDB" factory-bean="mopPDRepoDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The mopConfigInfoDB		-->
    <!--                            -->

	<bean id="mopConfigInfoDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="mopServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopconfiginfodbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopconfiginfodbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopconfiginfodbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${mopconfiginfodbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="mopConfigInfoDB" factory-bean="mopConfigInfoDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The mopSyncHelperDB		-->
    <!--                            -->

	<bean id="mopSyncHelperDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="mopServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopsynchelperdbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopsynchelperdbproxy.password}/value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopsynchelperdbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${mopsynchelperdbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="mopSyncHelperDB" factory-bean="mopSyncHelperDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The mopAdminDB				-->
    <!--                            -->

	<bean id="mopAdminDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="mopServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopadmindbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopadmindbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${mopadmindbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${mopadmindbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="mopAdminDB" factory-bean="mopAdminDBProxy" factory-method="getDb" />


    <!--                            -->
    <!-- various DBCollection		-->
    <!--                            -->

	<bean id="mopBaseCheckBridgeName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${mopbasecheckbridgename}</value>
		</constructor-arg>
	</bean>
	
	<bean id="mopBaseCheckBridge" factory-bean="mopConfigInfoDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="mopBaseCheckBridgeName" />
		</constructor-arg>
    </bean>

	<bean id="mopConfigInfoName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${mopconfiginfoname}</value>
		</constructor-arg>
	</bean>
	<bean id="mopConfigInfo" factory-bean="mopConfigInfoDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="mopConfigInfoName" />
		</constructor-arg>
    </bean>

	<bean id="parallelWriteAgentCacheName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${parallelwriteagentcachename}</value>
		</constructor-arg>
	</bean>
	
	<bean id="parallelWriteAgentCache" factory-bean="mopSyncHelperDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="parallelWriteAgentCacheName" />
		</constructor-arg>
    </bean>

	<bean id="parallelWriteAgentTaskName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${parallelwriteagenttaskname}</value>
		</constructor-arg>
	</bean>
	
	<bean id="parallelWriteAgentTask" factory-bean="mopSyncHelperDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="parallelWriteAgentTaskName" />
		</constructor-arg>
    </bean>


</beans>
