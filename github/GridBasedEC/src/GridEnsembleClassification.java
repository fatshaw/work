public class GridEnsembleClassification extends EnsembleClassification
{
	public void predict() throws Exception
	{
		GridDataManager gdm = (GridDataManager)testData;
		while(true){
			GridInstance [] gridInstances = gdm.getGridChunkData(1,PublicVariable.CHUNKSIZE);
			if(gridInstances == null){
				if(PublicVariable.DATASTREAM==true){
					Util.info("sleep for data");
					Util.debug("remaining = "+testData.remaining());
					Util.debug("predictnum = "+predictNum + "correctNum="+correctNum +",ratio="+correctNum/predictNum);
					Thread.sleep(PublicVariable.INTERVAL);
					continue;
				}
			}
			Util.debug("grid instance length = "+ gridInstances.length);
			for(int i = 0 ; i < gridInstances.length;++i){
				GridInstance gridInstance = gridInstances[i];
				gridInstance.average();
				classify(gridInstance.getInstance());
				gridInstance.setGridClassValue();
				curChunkData.delete(0);
				curChunkData.add(gridInstance.getInstance());
			}
			if(testData.hasNext()||PublicVariable.DATASTREAM){
				updateClassification();
			}
			else{
				break;
			}
		}
	}
	
}