import java.io.IOException;
import java.io.Reader;
import java.util.HashMap;
import java.util.Map;

import weka.core.Instance;
import weka.core.Instances;
import weka.filters.Filter;
import weka.filters.unsupervised.attribute.Normalize;

/**
 * Grid Data Manager,based on the Data Manager.Difference lies in that each
 * instance in the data stream is grid instance
 * 
 * @author Administrator
 * @edit:ÏÂÎç04:21:46
 */
public class GridDataManager extends DataManager {

	public GridDataManager(Reader reader) throws IOException {
		super(reader);
	}

	public GridDataManager(Instances allData) throws IOException {
		super(allData);
	}

	public GridInstance[] getGridChunkData(int r, int chunksize) {
		if (remaining() < chunksize * r && PublicVariable.DATASTREAM == true) {
			return null;
		}
		String coordinator = null;
		Map<String, GridInstance> map = new HashMap<String, GridInstance>();
		int len = PublicVariable.LEN;
		int cnt = 0;
		for (;;) {
			coordinator = "";
			Instance instance = getNextInstance();
			Util.debug("remaining = " + remaining());
			cnt++;
			// if(instance == null || cnt >= chunksize * r)break;
			if (instance == null || map.size() >= chunksize * r) {
				break;
			}
			for (int j = 0; j < instance.numAttributes() - 1; ++j) {
				int index = (int) (instance.value(j) * len);
				coordinator += index;
			}
			if (map.containsKey(coordinator)) {
				GridInstance mapInstance = map.get(coordinator);
				if (mapInstance.getInstance().classValue() != instance.classValue()) {
					Util.error("map error, " + mapInstance.getInstance().stringValue(PublicVariable.header.numAttributes() - 1) + ", "
							+ instance.stringValue(PublicVariable.header.numAttributes() - 1));
				}
				mapInstance.addInstance(instance);
			} else {
				GridInstance mapInstance = new GridInstance(instance);
				map.put(coordinator, mapInstance);
			}
		}
		if (map.size() == 0) {
			return null;
		}
		GridInstance[] gridInstance = new GridInstance[map.values().size()];
		gridInstance = map.values().toArray(gridInstance);
		return gridInstance;
	}

	@Override
	synchronized protected void addIntancesCache() {
		Normalize n = new Normalize();
		try {
			n.setInputFormat(this.nextData);
			this.nextData = Filter.useFilter(this.nextData, n);
			this.InstancesCache.add(this.nextData);
			this.nextData = new Instances(PublicVariable.header);
			Util.debug("normalize nextdata");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
