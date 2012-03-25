/*
 * Copyright (c) 2001 The Anthill Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

package peersim.edsim;

import peersim.core.Node;
import peersim.core.CommonState;
import peersim.config.Configuration;
import peersim.config.IllegalParameterException;

import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.PriorityQueue;
/**
 *  The Heap data structure used to maintain events "sorted" by 
 *  scheduled time and to obtain the next event to be executed.
 *  
 *  @author Alberto Montresor
 *  @version $Revision: 1.10 $
 */
public class PriorityHeap implements PriorityQ {

//--------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------

/** 
 * This parameter specifies how many
 * bits are used to order events that occur at the same time. Defaults
 * to 8. A value smaller than 8 causes an IllegalParameterException.
 * Higher values allow for a better discrimination, but reduce
 * the maximal time steps that can be simulated.
 * @config 
 */	
private static final String PAR_PBITS = "pbits";
private static final String PAR_PBITS_LEGACY = "simulation.timebits";

/** 
 * Specifies the initial capacity of the heap. Defaults to 65536.
 * @config 
 */	
private static final String PAR_SIZE = "size";

//--------------------------------------------------------------------------
// Fields
//--------------------------------------------------------------------------

// The following arrays are four heaps ordered by time. The alternative
// approach (i.e. to store event objects) requires much more memory,
// and based on some tests that I've done is not really much faster.


/** Number of elements */
private int size;

/** Singleton event object used to return (event, time, node, pid) tuples */
private final Event ev = new Event();
private static long itemId = 0;
private PriorityQueue<QueueItem> pq;

//--------------------------------------------------------------------------
// Contructor
//--------------------------------------------------------------------------

/**
 * Initializes a new heap using defaults.
 */
public PriorityHeap() {
	this(""); // "" is not a valid prefix for a component
}

//--------------------------------------------------------------------------

/**
 * Initializes a new heap using the configuration.
 */
public PriorityHeap(String prefix) {
	int size = 65536;
	try{
		size = Configuration.getInt(prefix+"."+PAR_SIZE,65536);
	}
	catch(Exception e){
		
	}
	pq = new PriorityQueue<QueueItem>(size);
}

//--------------------------------------------------------------------------
// Methods
//--------------------------------------------------------------------------

/**
 * Returns the current number of events in the system.
 */
public int size()
{
	return pq.size();
}

//--------------------------------------------------------------------------

/**
 * Add a new event, to be scheduled at the specified time.
 * 
 * @param time the time at which this event should be scheduled
 * @param event the object describing the event
 * @param node the node at which the event has to be delivered
 * @param pid the protocol that handles the event
 */
public void add(long time, Object event, Node node, byte pid) 
{
	//add(time,event,node,pid,CommonState.r.nextInt());
	add(time,event,node,pid,0);
}

//--------------------------------------------------------------------------

/**
 * Add a new event, to be scheduled at the specified time.
 * 
 * @param time the time at which this event should be scheduled
 * @param event the object describing the event
 * @param node the node at which the event has to be delivered
 * @param pid the protocol that handles the event
 */
public void add(long time, Object event, Node node, byte pid, long priority) 
{
	time += priority;
	QueueItem qi = new QueueItem();
	qi.event = event;
	qi.node = node;
	qi.time = time;
	qi.pid = pid;
	qi.id = itemId++;
	pq.offer(qi);
}

//--------------------------------------------------------------------------

/**
 * Removes the first event in the heap and returns it.
 * Note that, to avoid garbage collection, a singleton instance of
 * the Event class is used. This means that data contained in the
 * returned event are overwritten when a new invocation of this
 * method is performed.
 * @return first event or null if size is zero
 */
public Event removeFirst() {
	QueueItem qi = pq.poll();
	if(qi == null)return null;
	ev.event = qi.event;
	ev.time = qi.time;
	ev.node = qi.node;
	ev.pid = qi.pid;
	return ev;
}

//--------------------------------------------------------------------------

public long maxTime() { return Long.MAX_VALUE - 1;}

//--------------------------------------------------------------------------

public long maxPriority() { return Long.MAX_VALUE -1;}


public class QueueItem implements Comparable<QueueItem>
{
	public long time;
	public Object event;
	public byte pid;
	public Node node;
	public long id;
	@Override
	public int compareTo(QueueItem o) {
		if(this.time < o.time)
		{
			return -1;
		}
		else if(this.time == o.time)
		{
			if(this.id < o.id)
				return -1;
			else 
				return 1;
		}
		else 
		{
			return 1;
		}
	}
	
}

public static void main(String[]args)
{
	PriorityHeap ph = new PriorityHeap();
	ph.add(0, null, null, (byte)6);
	ph.add(1, null, null, (byte)2);
	ph.add(0, null, null, (byte)4);
	ph.add(0, null, null, (byte)3);
	ph.add(0, null, null, (byte)6);
	ph.add(2, null, null, (byte)1);
	Event ev = ph.removeFirst(); 
	while(ev != null){
		System.out.println("ev time ="+ev.time+"pid = "+ev.pid);
		ev = ph.removeFirst();
	}
	
	System.out.println("List");
	 
	List<QueueItem>list = new LinkedList<QueueItem>();
	QueueItem qi = ph.new QueueItem();
	qi.time = 1;
	qi.pid = 1;
	list.add(qi);
	qi = ph.new QueueItem();
	qi.time = 1;
	qi.pid = 2;
	list.add(qi);
	Collections.sort(list);
	Iterator<QueueItem> it = list.iterator();
	while(it.hasNext()){
		qi = it.next();
		System.out.println("ev time ="+qi.time+"pid = "+qi.pid);
	}
}

} // END PriorityQueue


