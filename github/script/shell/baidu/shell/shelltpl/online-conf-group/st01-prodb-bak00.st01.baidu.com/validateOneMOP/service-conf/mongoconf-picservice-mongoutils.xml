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

	<bean id="globalInfoServer" class="com.mongodb.Mongo">

		<constructor-arg type="java.lang.String">
			<value>${globalinfoserver.ip}</value>
		</constructor-arg>

		<constructor-arg type="int">
			<value>${globalinfoserver.port}</value>
		</constructor-arg>
	
	</bean>

    <!--                            -->
    <!-- The globalInfoDB			-->
    <!--                            -->

	<bean id="globalInfoDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="globalInfoServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${globalinfodbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${globalinfodbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${globalinfodbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${globalinfodbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="globalInfoDB" factory-bean="globalInfoDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- The globalInfoAdminDB		-->
    <!--                            -->

	<bean id="globalInfoAdminDBProxy" class="com.baidu.adrc.picservice.mongoutils.MongoDBProxy">

		<constructor-arg type="com.mongodb.Mongo">
    		<ref bean="globalInfoServer" />
    	</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${globalinfoadmindbproxy.username}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${globalinfoadmindbproxy.password}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.String">
			<value>${globalinfoadmindbproxy.dbname}</value>
		</constructor-arg>

		<constructor-arg type="java.lang.Boolean">
			<value>${globalinfoadmindbproxy.isprimary}</value>
		</constructor-arg>

	</bean>
	<bean id="globalInfoAdminDB" factory-bean="globalInfoDBProxy" factory-method="getDb" />

    <!--                            -->
    <!-- various DBCollection		-->
    <!--                            -->

	<bean id="incCounterName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${inccounter}</value>
		</constructor-arg>
	</bean>
	<bean id="incCounter" factory-bean="globalInfoDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="incCounterName" />
		</constructor-arg>
    </bean>

	<bean id="mopIncSyncTaskName" class="java.lang.String">
		<constructor-arg type="java.lang.String">
			<value>${mopincsynctask}</value>
		</constructor-arg>
	</bean>
	<bean id="mopIncSyncTask" factory-bean="globalInfoDB" factory-method="getCollection">
		<constructor-arg type="java.lang.String">
    		<ref bean="mopIncSyncTaskName" />
		</constructor-arg>
    </bean>

</beans>
