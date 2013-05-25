


<bean id="cpxIndexHelperName" class="java.lang.String">
	<constructor-arg type="java.lang.String">
	<value>${cpxIndexHelper}</value>
	</constructor-arg>
</bean>
<bean id="cpxIndexHelperTestName" class="java.lang.String">
	<constructor-arg type="java.lang.String">
	<value>${cpxIndexHelperT}</value>
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
