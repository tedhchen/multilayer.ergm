/*  File src/changestats.multilayer.c in package tc.ergmterms.
 *
 *  This software is distributed under the GPL-3 license.  It is free
 *  and open source.
 */
 #include "changestats.users.h"

D_CHANGESTAT_FN(d_duplexdyad){
	/* Declaring variables */
	int i, j, la, lb, type[N_CHANGE_STATS], layer_mem[N_NODES]; 
	int changes[5], edge_status[4];
	Vertex tail, head, t_neighbour, h_neighbour;
	Edge e1, e2;
	
	/* Breaking the INPUT_PARAM into the layers used, all the types to compute (1 to 5), and the membership of nodes (one of the two layers)*/
	/* The INPUT_PARAM array should have first and second elements as layers; up to five elements as statistics; and the rest as layer membership*/
	la = INPUT_PARAM[0]; lb = INPUT_PARAM[1];
	for(i = 0; i < N_CHANGE_STATS; i++){
		type[i] = INPUT_PARAM[2 + i];
	}
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[2 + N_CHANGE_STATS + i];
	}
	
	ZERO_ALL_CHANGESTATS(i);
	FOR_EACH_TOGGLE(i){
		tail = TAIL(i); head = HEAD(i);
		for(j = 0; j < 5; j++){changes[j] = 0;}
		for(j = 0; j < 4; j++){edge_status[j] = 0;}
		/* Check incident layers of the edge to determine edge type */
		if(layer_mem[tail - 1] == layer_mem[head - 1]){ /* Are they intralayer or interlayer edges?*/
			if(layer_mem[tail - 1] == la){ /* Edge on layer a */
				/* Determine the status of the tail-head dyad */
				edge_status[0] = IS_OUTEDGE(tail, head);
				edge_status[1] = IS_INEDGE(tail, head);
				
				/* Begin looping through interlayer outedges of tail*/
				STEP_THROUGH_OUTEDGES(tail, e1, t_neighbour){
					if(layer_mem[t_neighbour - 1] == lb){ /* Make sure it is an interlayer edge */
						/* Begin looping through interlayer outedge of head*/
						STEP_THROUGH_OUTEDGES(head, e2, h_neighbour){
							if(layer_mem[h_neighbour - 1] == lb && t_neighbour != h_neighbour){ /* Make sure it is an interlayer edge AND that is is not the same as the tail neighbour */
								/* Determine the status of the t_neighbour-h_neighbour dyad */
								edge_status[2] = IS_OUTEDGE(t_neighbour, h_neighbour);
								edge_status[3] = IS_INEDGE(t_neighbour, h_neighbour);
								/* Change statistic (turning the t-> edge from 0 to 1 */
								if(edge_status[1] == 0 && edge_status[2] == 1 && edge_status[3] == 0){changes[0] += 1;} /*_010*/
								if(edge_status[1] == 0 && edge_status[2] == 0 && edge_status[3] == 1){changes[1] += 1;} /*_001*/
								if(edge_status[1] == 0 && edge_status[2] == 1 && edge_status[3] == 1){changes[0] += 1; changes[1] += 1; changes[3] += 1;} /*_011*/
								if(edge_status[1] == 1 && edge_status[2] == 1 && edge_status[3] == 0){changes[0] += 1; changes[2] += 1;} /*_110*/
								if(edge_status[1] == 1 && edge_status[2] == 0 && edge_status[3] == 1){changes[1] += 1; changes[2] += 1;} /*_101*/
								if(edge_status[1] == 1 && edge_status[2] == 1 && edge_status[3] == 1){changes[0] += 1; changes[1] += 1; changes[2] += 2; changes[3] += 1; changes[4] += 1;} /*_111*/
							}
						}
					}
				}
			}else{ /* Edge on layer b */
				/* Determine status of the tail-head dyad */
				edge_status[2] = IS_OUTEDGE(tail, head);
				edge_status[3] = IS_INEDGE(tail, head);
        
				/* Begin loop based on cross layer edge from tail*/
				STEP_THROUGH_INEDGES(tail, e1, t_neighbour){
					if(layer_mem[t_neighbour - 1] == la){
						/* Begin loop based on cross layer edge from head*/
						STEP_THROUGH_INEDGES(head, e2, h_neighbour){
							if(layer_mem[h_neighbour - 1] == la && t_neighbour != h_neighbour){
								/* Determine the status of the dyad on layer b}}.*/
								edge_status[0] = IS_OUTEDGE(t_neighbour, h_neighbour);
								edge_status[1] = IS_INEDGE(t_neighbour, h_neighbour);
								/* Define the observed structure (based on everything but t->h) */
								if(edge_status[0] == 1 && edge_status[1] == 0 && edge_status[3] == 0){changes[0] += 1;} /*10_0*/
								if(edge_status[0] == 0 && edge_status[1] == 1 && edge_status[3] == 0){changes[1] += 1;} /*01_0*/
								if(edge_status[0] == 1 && edge_status[1] == 1 && edge_status[3] == 0){changes[0] += 1; changes[1] += 1; changes[2] += 1;} /*11_0*/
								if(edge_status[0] == 1 && edge_status[1] == 0 && edge_status[3] == 1){changes[0] += 1; changes[3] += 1;} /*10_1*/
								if(edge_status[0] == 0 && edge_status[1] == 1 && edge_status[3] == 1){changes[1] += 1; changes[3] += 1;} /*01_1*/
								if(edge_status[0] == 1 && edge_status[1] == 1 && edge_status[3] == 1){changes[0] += 1; changes[1] += 1; changes[2] += 1; changes[3] += 2; changes[4] += 1;} /*11_1*/
							}
						}
					}
				}
			}
		}else{  /* Edge crosses layers */
			if(layer_mem[tail - 1] == la){
				for(t_neighbour = 1; t_neighbour <= N_NODES; t_neighbour++){ /* Loop through tail neighbours*/
					for(h_neighbour = 1; h_neighbour <= N_NODES; h_neighbour++){ /* Loop through head neighbours*/
						if( (IS_OUTEDGE(tail, t_neighbour) || IS_INEDGE(tail, t_neighbour)) && /* Make sure the four nodes are part of a duplex dyad*/
							(IS_OUTEDGE(head, h_neighbour) || IS_INEDGE(head, h_neighbour)) && 
							(layer_mem[t_neighbour - 1] == la) && /* tail->t_neighbour must be an intralayer edge */
							(layer_mem[h_neighbour - 1] == lb) && /* head->h_neighbour must be an intralayer edge */
							(tail != t_neighbour) && (head != h_neighbour) && 
							(IS_OUTEDGE(t_neighbour, h_neighbour))){ /* t_neighbour and h_neighbour must be connected by an interlayer edge */
							/* Determining the status of ;the tail->t_neighbour dyad and the head->h_neighbour dyad*/							
							edge_status[0] = IS_OUTEDGE(tail, t_neighbour);
							edge_status[1] = IS_INEDGE(tail, t_neighbour);
							edge_status[2] = IS_OUTEDGE(head, h_neighbour);
							edge_status[3] = IS_INEDGE(head, h_neighbour);
							
							/* Define the observed structure (based on everything but t->h) */
							if(edge_status[0]== 1 && edge_status[1]== 1 && edge_status[2]== 1 && edge_status[3]== 1){changes[0] += 2; changes[1] += 2; changes[2] += 2; changes[3] += 2; changes[4] += 1;} /*I*/
							if(edge_status[0]== 0 && edge_status[1]== 1 && edge_status[2]== 1 && edge_status[3]== 1){changes[0] += 1; changes[1] += 1; changes[3] += 1;} /*H*/
							if(edge_status[0]== 1 && edge_status[1]== 0 && edge_status[2]== 1 && edge_status[3]== 1){changes[0] += 1; changes[1] += 1; changes[3] += 1;} /*H*/
							if(edge_status[0]== 1 && edge_status[1]== 1 && edge_status[2]== 1 && edge_status[3]== 0){changes[0] += 1; changes[1] += 1; changes[2] += 1;} /*G*/
							if(edge_status[0]== 1 && edge_status[1]== 1 && edge_status[2]== 0 && edge_status[3]== 1){changes[0] += 1; changes[1] += 1; changes[2] += 1;} /*G*/
							if(edge_status[0]== 1 && edge_status[1]== 0 && edge_status[2]== 0 && edge_status[3]== 1){changes[1] += 1;} /*F*/
							if(edge_status[0]== 0 && edge_status[1]== 1 && edge_status[2]== 1 && edge_status[3]== 0){changes[1] += 1;} /*F*/
							if(edge_status[0]== 1 && edge_status[1]== 0 && edge_status[2]== 1 && edge_status[3]== 0){changes[0] += 1;} /*E*/
							if(edge_status[0]== 0 && edge_status[1]== 1 && edge_status[2]== 0 && edge_status[3]== 1){changes[0] += 1;} /*E*/
							
						}
					}
				}
			}
		}
		/* Changing CHANGE_STAT */
		for(j = 0; j < N_CHANGE_STATS; j++){
			CHANGE_STAT[j] += IS_OUTEDGE(tail, head) ? -changes[type[j] - 1] : changes[type[j] - 1];		  
		}
    		// The current version of MPNet (v1.04) double counts certain cross-layer statistics; uncomment the following loop to reproduce exactly that.
		//for(j = 0; j < N_CHANGE_STATS; j++){
		//	CHANGE_STAT[j] *= 2;
		//}
		TOGGLE_IF_MORE_TO_COME(i);
	}
	UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_gwdsp) { 
	Edge e, f;
	int i, echange, ochange;
	int L2tu, L2uh, l, layer_mem[N_NODES];
	Vertex tail, head, u, v;
	double alpha, oneexpa, cumchange;
	
	/* *** don't forget tail -> head */    
	CHANGE_STAT[0] = 0.0;
	alpha = INPUT_PARAM[0];
	oneexpa = 1.0-exp(-alpha);
	l = INPUT_PARAM[1];
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[2 + i];
	}
	
	FOR_EACH_TOGGLE(i){
		tail = TAIL(i); head = HEAD(i);
		if(layer_mem[tail - 1] == l && layer_mem[head - 1] == l){
			cumchange=0.0;
			ochange = IS_OUTEDGE(tail, head) ? -1 : 0;
			echange = 2*ochange + 1;
			/* step through outedges of head */
			STEP_THROUGH_OUTEDGES(head, e, u){
				if (u != tail && layer_mem[u - 1] == l){
					L2tu=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					cumchange += pow(oneexpa,(double)L2tu);
				}
			}
			/* step through inedges of head */
			STEP_THROUGH_INEDGES(head, e, u){
				if (u != tail && layer_mem[u - 1] == l){
					L2tu=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					cumchange += pow(oneexpa,(double)L2tu);
				}
			}
			
			/* step through outedges of tail  */
			STEP_THROUGH_OUTEDGES(tail, e, u){
				if (u != head && layer_mem[u - 1] == l){
					L2uh=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
					}
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
					}
					cumchange += pow(oneexpa,(double)L2uh);
				}
			}
			/* step through inedges of tail */
			STEP_THROUGH_INEDGES(tail, e, u){
				if (u != head && layer_mem[u - 1] == l){
					L2uh=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
					}
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
					}
					cumchange += pow(oneexpa,(double)L2uh);
				}
			}
			cumchange  = echange*cumchange;
			(CHANGE_STAT[0]) += cumchange;
		}
		
		TOGGLE_IF_MORE_TO_COME(i);
	}
  UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_gwtdsp_layer) {
	Edge e, f;
	int i, echange, ochange, L2tu, L2uh, l, layer_mem[N_NODES];
	Vertex tail, head, u, v;
	double alpha, oneexpa, cumchange;

	CHANGE_STAT[0] = 0.0;
	alpha = INPUT_PARAM[0];
	oneexpa = 1.0-exp(-alpha);
	l = INPUT_PARAM[1];
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[2 + i];
	}

	/* *** don't forget tail -> head */    
	FOR_EACH_TOGGLE(i){
		tail=TAIL(i); head=HEAD(i);
		if(layer_mem[tail - 1] == l && layer_mem[head - 1] == l){
			cumchange=0.0;
			ochange = -IS_OUTEDGE(tail,head);
			echange = 2*ochange + 1;
			/* step through outedges of head */
			for(e = MIN_OUTEDGE(head); (u=OUTVAL(e))!=0; e=NEXT_OUTEDGE(e)) {
				if (u != tail && layer_mem[u - 1] == l){
					L2tu=ochange; /* L2tu will be # shrd prtnrs of (tail,u) not incl. head */
					/* step through inedges of u, incl. (head,u) itself */
					for(f = MIN_INEDGE(u); (v=INVAL(f))!=0; f=NEXT_INEDGE(f)) {
						if(IS_OUTEDGE(tail,v) && layer_mem[v - 1] == l) L2tu++;
					}
					cumchange += pow(oneexpa,(double)L2tu); /* sign corrected below */
				}
			}
			/* step through inedges of tail */
			for(e = MIN_INEDGE(tail); (u=INVAL(e))!=0; e=NEXT_INEDGE(e)) {
				if (u != head && layer_mem[u - 1] == l){
					L2uh=ochange; /* L2uh will be # shrd prtnrs of (u,head) not incl. tail */
					/* step through outedges of u , incl. (u,tail) itself */
					for(f = MIN_OUTEDGE(u);(v=OUTVAL(f))!=0; f=NEXT_OUTEDGE(f)){
						if(IS_OUTEDGE(v,head) && layer_mem[v - 1] == l) L2uh++;
					}
					cumchange += pow(oneexpa,(double)L2uh); /* sign corrected below */
				}
			}
			CHANGE_STAT[0] += echange * cumchange;
		}
		TOGGLE_IF_MORE_TO_COME(i);
	}
	UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_gwesp) { 
	Edge e, f;
	int i, echange, ochange;
	int L2th, L2tu, L2uh, l, layer_mem[N_NODES];
	Vertex tail, head, u, v;
	double alpha, oneexpa, cumchange;
	
	CHANGE_STAT[0] = 0.0;
	alpha = INPUT_PARAM[0];
	oneexpa = 1.0-exp(-alpha);
	
	l = INPUT_PARAM[1];
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[2 + i];
	}
	
	/* *** don't forget tail -> head */    
	FOR_EACH_TOGGLE(i){
		tail=TAIL(i); head=HEAD(i);
		if(layer_mem[tail - 1] == l && layer_mem[head - 1] == l){
			cumchange=0.0;
			L2th=0;
			ochange = IS_OUTEDGE(tail, head) ? -1 : 0;
			echange = 2*ochange + 1;
			/* step through outedges of head  */
			STEP_THROUGH_OUTEDGES(head, e, u){
				if (IS_UNDIRECTED_EDGE(u, tail) && layer_mem[u - 1] == l){
					L2th++;
					L2tu=ochange;
					L2uh=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					cumchange += pow(oneexpa,(double)L2tu) +
					pow(oneexpa,(double)L2uh) ;
				}
			}
			
			/* step through inedges of head */
			STEP_THROUGH_INEDGES(head, e, u){
				if (IS_UNDIRECTED_EDGE(u, tail) && layer_mem[u - 1] == l){
					L2th++;
					L2tu=ochange;
					L2uh=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_UNDIRECTED_EDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
						if(IS_UNDIRECTED_EDGE(v, tail) && layer_mem[v - 1] == l) L2tu++;
					}
					cumchange += pow(oneexpa,(double)L2tu) +
					pow(oneexpa,(double)L2uh) ;
				}
			}
			
			if(alpha < 100.0){
				cumchange += exp(alpha)*(1.0-pow(oneexpa,(double)L2th)) ;
			} else {
				cumchange += (double)L2th;
			}
			cumchange  = echange*cumchange;
			(CHANGE_STAT[0]) += cumchange;
		}
		TOGGLE_IF_MORE_TO_COME(i);
	}
	UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_gwtesp_layer) { 
	Edge e, f;
	int i, echange, ochange;
	int L2th, L2tu, L2uh, l, layer_mem[N_NODES];
	Vertex tail, head, u, v;
	double alpha, oneexpa, cumchange;
  
	CHANGE_STAT[0] = 0.0;
	alpha = INPUT_PARAM[0];
	oneexpa = 1.0-exp(-alpha);
	l = INPUT_PARAM[1];
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[2 + i];
	}
	
	/* *** don't forget tail -> head */    
	FOR_EACH_TOGGLE(i){
		tail = TAIL(i); head = HEAD(i);
		if(layer_mem[tail - 1] == l && layer_mem[head - 1] == l){
			cumchange=0.0;
			L2th=0;
			ochange = IS_OUTEDGE(tail, head) ? -1 : 0;
			echange = 2*ochange + 1;
			/* step through outedges of head  */
			STEP_THROUGH_OUTEDGES(head, e, u){
				if (IS_OUTEDGE(tail, u) && layer_mem[u - 1] == l){
					L2tu=ochange;
					/* step through inedges of u */
					STEP_THROUGH_INEDGES(u, f, v){
						if(IS_OUTEDGE(tail, v) && layer_mem[v - 1] == l) L2tu++;
					}
					cumchange += pow(oneexpa,(double)L2tu);
				}
			}
			
			/* step through inedges of head */
			STEP_THROUGH_INEDGES(head, e, u){
				if (IS_OUTEDGE(tail, u)){
					L2th++;
				}
				if (IS_OUTEDGE(u, tail) && layer_mem[u - 1] == l){
					L2uh=ochange;
					/* step through outedges of u */
					STEP_THROUGH_OUTEDGES(u, f, v){
						if(IS_OUTEDGE(v, head) && layer_mem[v - 1] == l) L2uh++;
					}
					cumchange += pow(oneexpa,(double)L2uh) ;
				}
			}
			
			if(alpha < 100.0){
				cumchange += exp(alpha)*(1.0-pow(oneexpa,(double)L2th)) ;
			}else{
				cumchange += (double)L2th;
			}
			cumchange  = echange*cumchange;
			(CHANGE_STAT[0]) += cumchange;
		}
		TOGGLE_IF_MORE_TO_COME(i);
	}
	UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_nodeifactor_layer) { 
  double s;
  Vertex head, tail;
  int i, l1, l2, layer_mem[N_NODES];
  
  l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
  l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];
  for(i = 0; i < N_NODES; i++){
	  layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
  }
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
    head = HEAD(i); tail = TAIL(i);
	if(l2 == 0){
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1){
			s = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
			int headpos = INPUT_PARAM[head-1];
			if (headpos!=-1) CHANGE_STAT[headpos] += s;
		}
	}else{
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2){
			s = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
			int headpos = INPUT_PARAM[head-1];
			if (headpos!=-1) CHANGE_STAT[headpos] += s;
		}
	}
    TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_nodeofactor_layer) { 
  double s;
  Vertex head, tail;
  int i, l1, l2, layer_mem[N_NODES];
  
  l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
  l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];

  for(i = 0; i < N_NODES; i++){
	  layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
  }
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
    head = HEAD(i); tail = TAIL(i);
	if(l2 == 0){
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1){
			s = IS_OUTEDGE(tail, HEAD(i)) ? -1.0 : 1.0;
			int tailpos = INPUT_PARAM[tail-1];
			if (tailpos!=-1) CHANGE_STAT[tailpos] += s;
		}
	}else{
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2){
			s = IS_OUTEDGE(tail, HEAD(i)) ? -1.0 : 1.0;
			int tailpos = INPUT_PARAM[tail-1];
			if (tailpos!=-1) CHANGE_STAT[tailpos] += s;
		}
	}
    TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_nodefactor_layer) { 
  double s;
  Vertex tail, head;
  int i, l1, l2, layer_mem[N_NODES];
  
  l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
  l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];
  
  for(i = 0; i < N_NODES; i++){
	  layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
  }
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
    tail = TAIL(i); head = HEAD(i);
	if(l2 == 0){
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1){
			s = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
			int tailpos = INPUT_PARAM[tail-1];
			int headpos = INPUT_PARAM[head-1];
			if (tailpos!=-1) CHANGE_STAT[tailpos] += s;
			if (headpos!=-1) CHANGE_STAT[headpos] += s;
		}
	}else{
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2){
			s = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
			int tailpos = INPUT_PARAM[tail-1];
			int headpos = INPUT_PARAM[head-1];
			if (tailpos!=-1) CHANGE_STAT[tailpos] += s;
			if (headpos!=-1) CHANGE_STAT[headpos] += s;
		}
	}
    TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_nodecov_layer) { 
  double sum;
  Vertex tail, head;
  int i, edgeflag, l1, l2, layer_mem[N_NODES];
  
  l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
  l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];
  
  for(i = 0; i < N_NODES; i++){
	  layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
  }

  /* *** don't forget tail -> head */    
  CHANGE_STAT[0] = 0.0;
  FOR_EACH_TOGGLE(i) {
	tail = TAIL(i); head = HEAD(i);
	if(l2 ==0){
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1){
			edgeflag=IS_OUTEDGE(tail, head);
			sum = INPUT_PARAM[tail-1] + INPUT_PARAM[head-1];
			CHANGE_STAT[0] += edgeflag ? -sum : sum;
		}
	}else{
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2){
			edgeflag=IS_OUTEDGE(tail, head);
			sum = INPUT_PARAM[tail-1] + INPUT_PARAM[head-1];
			CHANGE_STAT[0] += edgeflag ? -sum : sum;
		}
	}
    TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_degree_layer) { 
  /*  The inputparams are assumed to be set up as follows:
  The first nstats values are the values of degree
  The values following the first nstats values are the nodal attributes.
  */
  int i, j, echange, tailattr, headattr;
  Vertex tail, head, taildeg, headdeg, deg, v, l1, l2, layer_mem[N_NODES];
  Edge e;
  
  l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
  l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];
  
  for(i = 0; i < N_NODES; i++){
	  layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
  }
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
    tail=TAIL(i);
    head=HEAD(i);
	
	tailattr = layer_mem[tail - 1];
	headattr = layer_mem[head - 1];
	
	if(l1 == l2){
		if (tailattr == l1 && headattr == l2) {
			echange = IS_OUTEDGE(tail, head) ? -1:1;
			taildeg=headdeg=0;
			STEP_THROUGH_OUTEDGES(tail, e, v) { taildeg += (layer_mem[v - 1] == l2); }
			STEP_THROUGH_INEDGES(tail, e, v) { taildeg += (layer_mem[v - 1] == l2); }
			STEP_THROUGH_OUTEDGES(head, e, v) { headdeg += (layer_mem[v - 1] == l1); }
			STEP_THROUGH_INEDGES(head, e, v) { headdeg += (layer_mem[v - 1] == l1); }
			for(j = 0; j < N_CHANGE_STATS; j++) {
				deg = INPUT_PARAM[j];
				CHANGE_STAT[j] += (taildeg + echange == deg) - (taildeg == deg);
				CHANGE_STAT[j] += (headdeg + echange == deg) - (headdeg == deg);
			}
		}
	}else{
		if (tailattr == l1 && headattr == l2) {
			echange = IS_OUTEDGE(tail, head) ? -1:1;
			taildeg=headdeg=0;
			STEP_THROUGH_OUTEDGES(tail, e, v) { taildeg += (layer_mem[v - 1] == l2); }
			STEP_THROUGH_INEDGES(tail, e, v) { taildeg += (layer_mem[v - 1] == l2); }
			STEP_THROUGH_OUTEDGES(head, e, v) { headdeg += (layer_mem[v - 1] == l1); }
			STEP_THROUGH_INEDGES(head, e, v) { headdeg += (layer_mem[v - 1] == l1); }
			for(j = 0; j < N_CHANGE_STATS; j++) {
				deg = INPUT_PARAM[j];
				CHANGE_STAT[j] += (taildeg + echange == deg) - (taildeg == deg);
				CHANGE_STAT[j] += (headdeg + echange == deg) - (headdeg == deg);
			}
		}
		if (tailattr == l2 && headattr == l1) { 
			echange = IS_OUTEDGE(tail, head) ? -1:1;
			taildeg=headdeg=0;
			STEP_THROUGH_OUTEDGES(tail, e, v) { taildeg += (layer_mem[v - 1] == l1); }
			STEP_THROUGH_INEDGES(tail, e, v) { taildeg += (layer_mem[v - 1] == l1); }
			STEP_THROUGH_OUTEDGES(head, e, v) { headdeg += (layer_mem[v - 1] == l2); }
			STEP_THROUGH_INEDGES(head, e, v) { headdeg += (layer_mem[v - 1] == l2); }
			for(j = 0; j < N_CHANGE_STATS; j++) {
				deg = INPUT_PARAM[j];
				CHANGE_STAT[j] += (taildeg + echange == deg) - (taildeg == deg);
				CHANGE_STAT[j] += (headdeg + echange == deg) - (headdeg == deg);
			}
		}
	}	
    TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);
} 

D_CHANGESTAT_FN(d_altkstar_fixed_layer) { 
  int i, isedge;
  double lambda, oneexpl, change;
  Vertex v, tail, head, taild, headd, l1, l2, layer_mem[N_NODES];
  Edge e;
  
  l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
  l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];
  
  for(i = 0; i < N_NODES; i++){
	  layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
  }
  
  change = 0.0;
  lambda = INPUT_PARAM[0];
  oneexpl = 1.0-1.0/lambda;

  /* *** don't forget tail -> head */
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
	tail=TAIL(i); head=HEAD(i);
	taild = headd = 0;
	
	if(l2 == 0){
		if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1){
			isedge = IS_OUTEDGE(tail, head);
		
			STEP_THROUGH_OUTEDGES(tail, e, v) { taild += (layer_mem[v - 1] == l1); }
			STEP_THROUGH_INEDGES(tail, e, v) { taild += (layer_mem[v - 1] == l1); }

			STEP_THROUGH_OUTEDGES(head, e, v) { headd += (layer_mem[v - 1] == l1); }
			STEP_THROUGH_INEDGES(head, e, v) { headd += (layer_mem[v - 1] == l1); }
		
			taild -= isedge;
			headd -= isedge;
		
			if(taild!=0){
				change += (1-2*isedge)*(1.0-pow(oneexpl,(double)taild));
			}
			if(headd!=0){
				change += (1-2*isedge)*(1.0-pow(oneexpl,(double)headd));
			}
			TOGGLE_IF_MORE_TO_COME(i); /* Needed in case of multiple toggles */
		}
	}else{
		if((layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2) || (layer_mem[tail - 1] == l2 && layer_mem[head - 1] == l1)){
			isedge = IS_OUTEDGE(tail, head) + IS_INEDGE(tail, head);
			taild = headd = 0;
					
			STEP_THROUGH_OUTEDGES(tail, e, v) { taild += (layer_mem[v - 1] == layer_mem[head - 1]); }
			STEP_THROUGH_INEDGES(tail, e, v) { taild += (layer_mem[v - 1] == layer_mem[head - 1]); }
					
			STEP_THROUGH_OUTEDGES(head, e, v) { headd += (layer_mem[v - 1] == layer_mem[tail - 1]); }
			STEP_THROUGH_INEDGES(head, e, v) { headd += (layer_mem[v - 1] == layer_mem[tail - 1]); }
					
			taild -= isedge;
			headd -= isedge;
		
			if(l1 < l2 && taild!=0){
				change += (1-2*isedge)*(1.0-pow(oneexpl,(double)taild));
			}
			
			if(l1 > l2 && headd!=0){
				change += (1-2*isedge)*(1.0-pow(oneexpl,(double)headd));
			}

			TOGGLE_IF_MORE_TO_COME(i); /* Needed in case of multiple toggles */
		}
	}
  }
  CHANGE_STAT[0] = change*lambda;  
  UNDO_PREVIOUS_TOGGLES(i); /* Needed on exit in case of multiple toggles */
}

D_CHANGESTAT_FN(d_kstar_layer) { 
	double change, taild, headd;
	int isedge, i, j, kmo, l1, l2;
	Edge e;
	Vertex tail, head, v, layer_mem[N_NODES], attribs[N_NODES];
	int atts, intralayer;
	double tailattr;
	
	l1 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 2];
	l2 = INPUT_PARAM[N_INPUT_PARAMS - N_NODES - 1];
	
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
	}
	
	atts = (N_INPUT_PARAMS > (N_CHANGE_STATS + N_NODES + 2));
	intralayer = (l2 == 0);
	
	/* *** don't forget tail -> head */    
	ZERO_ALL_CHANGESTATS(i);
	if(intralayer == 1){
		if(atts == 1){
			for(i = 0; i < N_NODES; i++){
				attribs[i] = INPUT_PARAM[N_CHANGE_STATS + i];
			}
			
			FOR_EACH_TOGGLE(i){
				tail = TAIL(i); head = HEAD(i);
				tailattr = attribs[tail -1];
				if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1 && attribs[head - 1] == tailattr){
					isedge = IS_OUTEDGE(tail, head);
					taild = headd = 0;
					
					STEP_THROUGH_OUTEDGES(tail, e, v) { taild += ((layer_mem[v - 1] == l1) && (attribs[v - 1] == tailattr)); }
					STEP_THROUGH_INEDGES(tail, e, v) { taild += ((layer_mem[v - 1] == l1) && (attribs[v - 1] == tailattr)); }

					STEP_THROUGH_OUTEDGES(head, e, v) { headd += ((layer_mem[v - 1] == l1) && (attribs[v - 1] == tailattr)); }
					STEP_THROUGH_INEDGES(head, e, v) { headd += ((layer_mem[v - 1] == l1) && (attribs[v - 1] == tailattr)); }
				
					taild -= isedge;
					headd -= isedge;
					
					for(j=0; j < N_CHANGE_STATS; j++) {
						kmo = INPUT_PARAM[j] - 1;
						change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
						CHANGE_STAT[j] += (isedge ? - change : change); 
					}
					TOGGLE_IF_MORE_TO_COME(i);
				}
			}
		}else{
			FOR_EACH_TOGGLE(i){
				tail = TAIL(i); head = HEAD(i);
				if(layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l1){
					isedge = IS_OUTEDGE(tail, head);
					taild = headd = 0;
										
					STEP_THROUGH_OUTEDGES(tail, e, v) { taild += (layer_mem[v - 1] == l1); }
					STEP_THROUGH_INEDGES(tail, e, v) { taild += (layer_mem[v - 1] == l1); }

					STEP_THROUGH_OUTEDGES(head, e, v) { headd += (layer_mem[v - 1] == l1); }
					STEP_THROUGH_INEDGES(head, e, v) { headd += (layer_mem[v - 1] == l1); }
				
					taild -= isedge;
					headd -= isedge;
					
					for(j=0; j < N_CHANGE_STATS; j++) {
						kmo = INPUT_PARAM[j] - 1;
						change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
						CHANGE_STAT[j] += (isedge ? - change : change); 
					}
					TOGGLE_IF_MORE_TO_COME(i);
				}
			}
		}
	}else{
		if(atts == 1){
			for(i = 0; i < N_NODES; i++){
				attribs[i] = INPUT_PARAM[N_CHANGE_STATS + i];
			}
			FOR_EACH_TOGGLE(i){
				tail = TAIL(i); head = HEAD(i);
				tailattr = attribs[tail -1];
				if(((layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2) || (layer_mem[tail - 1] == l2 && layer_mem[head - 1] == l1)) && attribs[head - 1] == tailattr){
					isedge = IS_OUTEDGE(tail, head) + IS_INEDGE(tail, head);
					taild = headd = 0;
					
					STEP_THROUGH_OUTEDGES(tail, e, v) { taild += ((layer_mem[v - 1] == layer_mem[head - 1]) && (attribs[v - 1] == tailattr)); }
					STEP_THROUGH_INEDGES(tail, e, v) { taild += ((layer_mem[v - 1] == layer_mem[head - 1]) && (attribs[v - 1] == tailattr)); }
					
					STEP_THROUGH_OUTEDGES(head, e, v) { headd += ((layer_mem[v - 1] == layer_mem[tail - 1]) && (attribs[v - 1] == tailattr)); }
					STEP_THROUGH_INEDGES(head, e, v) { headd += ((layer_mem[v - 1] == layer_mem[tail - 1]) && (attribs[v - 1] == tailattr)); }
					
					taild -= isedge;
					headd -= isedge;
					
					for(j=0; j < N_CHANGE_STATS; j++) {
						kmo = INPUT_PARAM[j] - 1;
						change = (l1 < l2 ? CHOOSE(taild, kmo) : CHOOSE(headd, kmo)); 
						CHANGE_STAT[j] += (isedge ? - change : change); 
					}
					TOGGLE_IF_MORE_TO_COME(i); /* Needed in case of multiple toggles */
				}
			}
		}else{
			FOR_EACH_TOGGLE(i){
				tail = TAIL(i); head = HEAD(i);
				if((layer_mem[tail - 1] == l1 && layer_mem[head - 1] == l2) || (layer_mem[tail - 1] == l2 && layer_mem[head - 1] == l1)){
					isedge = IS_OUTEDGE(tail, head) + IS_INEDGE(tail, head);
					taild = headd = 0;
					
					STEP_THROUGH_OUTEDGES(tail, e, v) { taild += (layer_mem[v - 1] == layer_mem[head - 1]); }
					STEP_THROUGH_INEDGES(tail, e, v) { taild += (layer_mem[v - 1] == layer_mem[head - 1]); }
					
					STEP_THROUGH_OUTEDGES(head, e, v) { headd += (layer_mem[v - 1] == layer_mem[tail - 1]); }
					STEP_THROUGH_INEDGES(head, e, v) { headd += (layer_mem[v - 1] == layer_mem[tail - 1]); }
					
					taild -= isedge;
					headd -= isedge;
					
					for(j=0; j < N_CHANGE_STATS; j++) {
						kmo = INPUT_PARAM[j] - 1;
						change = (l1 < l2 ? CHOOSE(taild, kmo) : CHOOSE(headd, kmo)); 
						CHANGE_STAT[j] += (isedge ? - change : change); 
					}
					TOGGLE_IF_MORE_TO_COME(i); /* Needed in case of multiple toggles */
				}
			}
		}
	}
	UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_altkstar_fixed_crosslayer) { 
	int i, isedge, l11, l12, l21, l22, tl, hl;
	double lambda, oneexpl, change;
	Vertex v, tail, head, taild, headd, layer_mem[N_NODES];
	Edge e;
	
	l11 = INPUT_PARAM[1];
	l12 = INPUT_PARAM[2];
	l21 = INPUT_PARAM[3];
	l22 = INPUT_PARAM[4];
  
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[N_INPUT_PARAMS - N_NODES + i];
	}
  
	change = 0.0;
	lambda = INPUT_PARAM[0];
	oneexpl = 1.0-1.0/lambda;

	/* *** don't forget tail -> head */
	CHANGE_STAT[0] = 0.0;
	FOR_EACH_TOGGLE(i) {
		tail=TAIL(i); head=HEAD(i);
		taild = headd = 0;
		tl = layer_mem[tail - 1]; hl = layer_mem[head - 1];
		isedge = IS_OUTEDGE(tail, head);
		
		if(tl == hl){
			if(tl == l11 && tl == l12){
				STEP_THROUGH_OUTEDGES(tail, e, v){taild += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
				STEP_THROUGH_INEDGES(tail, e, v){taild += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
				
				STEP_THROUGH_OUTEDGES(head, e, v){headd += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
				STEP_THROUGH_INEDGES(head, e, v){headd += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
				
				taild -= isedge;
				headd -= isedge;
			}
			if(tl == l21 && tl == l22){
				STEP_THROUGH_OUTEDGES(tail, e, v){taild += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
				STEP_THROUGH_INEDGES(tail, e, v){taild += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
				
				STEP_THROUGH_OUTEDGES(head, e, v){headd += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
				STEP_THROUGH_INEDGES(head, e, v){headd += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
				
				taild -= isedge;
				headd -= isedge;
			}
		}else{
			if((tl == l11 && hl == l12) || (hl == l11 && tl == l12)){
				if(tl == l21){
					STEP_THROUGH_OUTEDGES(tail, e, v){taild += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
					STEP_THROUGH_INEDGES(tail, e, v){taild += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
					taild -= isedge;
				}else{
					STEP_THROUGH_OUTEDGES(head, e, v){headd += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
					STEP_THROUGH_INEDGES(head, e, v){headd += (layer_mem[v - 1] == l11 || layer_mem[v - 1] == l12);}
					headd -= isedge;
				}
			}
			if((tl == l21 && hl == l22) || (hl == l21 && tl == l22)){
				if(tl == l11){
					STEP_THROUGH_OUTEDGES(tail, e, v){taild += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
					STEP_THROUGH_INEDGES(tail, e, v){taild += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
					taild -= isedge;
				}else{
					STEP_THROUGH_OUTEDGES(head, e, v){headd += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
					STEP_THROUGH_INEDGES(head, e, v){headd += (layer_mem[v - 1] == l21 || layer_mem[v - 1] == l22);}
					headd -= isedge;
				}
			}
		}
		
		if(taild!=0){
			change += (1-2*isedge)*(1.0-pow(oneexpl,(double)taild));
		}
		if(headd!=0){
			change += (1-2*isedge)*(1.0-pow(oneexpl,(double)headd));
		}
		TOGGLE_IF_MORE_TO_COME(i); /* Needed in case of multiple toggles */
	}
	
	CHANGE_STAT[0] = change*lambda;
	UNDO_PREVIOUS_TOGGLES(i); /* Needed on exit in case of multiple toggles */
}

D_CHANGESTAT_FN(d_threetrail_crosslayer) { 
	int i, j, k, l11, l12, l21, l22, l31, l32, incident;
	int isedge, TTbase, TT2, TT3, inTT, change;
	int uv_used[N_NODES][N_NODES];
	Edge e1, e2, e3, e4;
	Vertex t, h, u, v, tl, hl, ul, vl, layer_mem[N_NODES];
	Vertex w1, w2, hout_used[N_NODES], tout_used[N_NODES];
	
	l11 = INPUT_PARAM[0]; l12 = INPUT_PARAM[1];
	l21 = INPUT_PARAM[2]; l22 = INPUT_PARAM[3];
	l31 = INPUT_PARAM[4]; l32 = INPUT_PARAM[5];
	incident = INPUT_PARAM[N_INPUT_PARAMS - 1];
	
	change = 0;
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[6 + i];
	}
	
	ZERO_ALL_CHANGESTATS(i);
	FOR_EACH_TOGGLE(i){
		t = TAIL(i); h = HEAD(i);
		tl = layer_mem[t - 1]; hl = layer_mem[h - 1];
		TTbase = 0; 
		TT2 = 0;
		TT3 = 0;
		inTT = 0;
		isedge = IS_OUTEDGE(t, h);
		
		if(l11 == l12){ /* intra-inter-intra */
			if((tl == l11 && hl == l12) || (tl == l31 && hl == l32)){
				for(j = 0; j < N_NODES; j++){
					for(k = 0; k < N_NODES; k++){
						uv_used[j][k] = 0;
					}
				}
				STEP_THROUGH_OUTEDGES(t, e1, u){ /* t -> u */ 
					ul = layer_mem[u - 1];
					if((ul == l21 || ul == l22) && ul != tl){
						STEP_THROUGH_OUTEDGES(u, e2, v){ /* t -> u -> v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
						STEP_THROUGH_INEDGES(u, e2, v){ /* t -> u <- v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
					}
					if(inTT > 0){
						change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
					}
					TT2 = 0;
					inTT = 0;
				}
				STEP_THROUGH_INEDGES(t, e1, u){ /* t <- u */
					ul = layer_mem[u - 1];
					if((ul == l21 || ul == l22) && ul != tl){
						STEP_THROUGH_OUTEDGES(u, e2, v){ /* t <- u -> v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
						STEP_THROUGH_INEDGES(u, e2, v){ /* t <- u <- v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(t, e3, w1){
										if(w1 != h && layer_mem[w1 - 1] == tl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == tl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
					}
					if(inTT > 0){
						change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
					}
					TT2 = 0;
					inTT = 0;
				}
				STEP_THROUGH_OUTEDGES(h, e1, u){ /* h -> u */
					ul = layer_mem[u - 1];
					if((ul == l21 || ul == l22) && ul != hl){
						STEP_THROUGH_OUTEDGES(u, e2, v){ /* h -> u -> v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
						STEP_THROUGH_INEDGES(u, e2, v){ /* h -> u <- v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
					}
					if(inTT > 0){
						change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
					}
					TT2 = 0;
					inTT = 0;
				}
				STEP_THROUGH_INEDGES(h, e1, u){ /* h <- u */
					ul = layer_mem[u - 1];
					if((ul == l21 || ul == l22) && ul != hl){
						STEP_THROUGH_OUTEDGES(u, e2, v){ /* h <- u -> v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
						STEP_THROUGH_INEDGES(u, e2, v){ /* h <- u <- v */
							vl = layer_mem[v - 1];
							if(vl == ul){
								TTbase++;
								inTT++;
								if(incident == 1){
									STEP_THROUGH_OUTEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									STEP_THROUGH_INEDGES(h, e3, w1){
										if(w1 != t && layer_mem[w1 - 1] == hl){
											TT2++;
											TT3++;
										}
									}
									/* Checking whether uv edge has been counted before */
									if(uv_used[u - 1][v - 1] != 1){
										if(TT3 == 0){
											STEP_THROUGH_OUTEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(u, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_OUTEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
											STEP_THROUGH_INEDGES(v, e3, w1){
												if(TT3 > 0){break;}
												if(layer_mem[w1 - 1] == hl){
													STEP_THROUGH_OUTEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
													STEP_THROUGH_INEDGES(w1, e4, w2){
														if(TT3 > 0){break;}
														if((layer_mem[w2 - 1] == layer_mem[w1 - 1]) && !((w1 == t && w2 == h) || (w1 == h && w2 == t))){TT3++;}
													}
												}
											}
										}
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
										uv_used[u - 1][v - 1] = 1;
										uv_used[v - 1][u - 1] = 1;
									}
									TT3 = 0;
								}
							}
						}
					}
					if(inTT > 0){
						change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
					}
					TT2 = 0;
					inTT = 0;
				}
			}
			if((tl == l21 && hl == l22) || (hl == l21 && tl == l22)){
				for(j = 0; j < N_NODES; j++){
					tout_used[j] = 0;
					hout_used[j] = 0;
				}
				STEP_THROUGH_OUTEDGES(t, e1, u){ /* t -> u */
					ul = layer_mem[u - 1];
					if(ul == tl){
						STEP_THROUGH_OUTEDGES(h, e2, v){ /* t -> u; h -> v */
							vl = layer_mem[v - 1];
							if(vl == hl){
								TTbase++;
								if(incident == 1){
									if(IS_OUTEDGE(u, v) || IS_INEDGE(u, v) || IS_OUTEDGE(t, v) || IS_INEDGE(t, v) || IS_OUTEDGE(h, u) || IS_INEDGE(h, u)){
										TT2++;
										TT3++;
									}else{
										STEP_THROUGH_OUTEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
									}
									if(tout_used[u - 1] == 0){
										change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
									}
									if(hout_used[v - 1] == 0){
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
									}
									tout_used[u - 1] = 1;
									hout_used[v - 1] = 1;
									TT2 = 0;
									TT3 = 0;
								}
							}
						}
						STEP_THROUGH_INEDGES(h, e2, v){ /* t -> u; h <- v */
							vl = layer_mem[v - 1];
							if(vl == hl){
								TTbase++;
								if(incident == 1){
									if(IS_OUTEDGE(u, v) || IS_INEDGE(u, v) || IS_OUTEDGE(t, v) || IS_INEDGE(t, v) || IS_OUTEDGE(h, u) || IS_INEDGE(h, u)){
										TT2++;
										TT3++;
									}else{
										STEP_THROUGH_OUTEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
									}
									if(tout_used[u - 1] == 0){
										change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
									}
									if(hout_used[v - 1] == 0){
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
									}
									tout_used[u - 1] = 1;
									hout_used[v - 1] = 1;
									TT2 = 0;
									TT3 = 0;
								}
							}
						}
					}
				}
				STEP_THROUGH_INEDGES(t, e1, u){ /* t <- u */
					ul = layer_mem[u - 1];
					if(ul == tl){
						STEP_THROUGH_OUTEDGES(h, e2, v){ /* t <- u; h -> v */
							vl = layer_mem[v - 1];
							if(vl == hl){
								TTbase++;
								if(incident == 1){
									if(IS_OUTEDGE(u, v) || IS_INEDGE(u, v) || IS_OUTEDGE(t, v) || IS_INEDGE(t, v) || IS_OUTEDGE(h, u) || IS_INEDGE(h, u)){
										TT2++;
										TT3++;
									}else{
										STEP_THROUGH_OUTEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
									}
									if(tout_used[u - 1] == 0){
										change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
									}
									if(hout_used[v - 1] == 0){
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
									}
									tout_used[u - 1] = 1;
									hout_used[v - 1] = 1;
									TT2 = 0;
									TT3 = 0;
								}
							}
						}
						STEP_THROUGH_INEDGES(h, e2, v){ /* t <- u; h <- v */
							vl = layer_mem[v - 1];
							if(vl == hl){
								TTbase++;
								if(incident == 1){
									if(IS_OUTEDGE(u, v) || IS_INEDGE(u, v) || IS_OUTEDGE(t, v) || IS_INEDGE(t, v) || IS_OUTEDGE(h, u) || IS_INEDGE(h, u)){
										TT2++;
										TT3++;
									}else{
										STEP_THROUGH_OUTEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(t, e3, w1){
											if(TT2 > 0){break;}
											if(w1 != h && layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 - 1] == hl){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(h, e3, w1){
											if(TT3 > 0){break;}
											if(w1 != t && layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 - 1] == tl){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(u, e3, w1){
											if(TT2 > 0){break;}
											if(layer_mem[w1 - 1] == hl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT2 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT2++;
													}
												}
											}
										}
										STEP_THROUGH_OUTEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
										STEP_THROUGH_INEDGES(v, e3, w1){
											if(TT3 > 0){break;}
											if(layer_mem[w1 - 1] == tl){
												STEP_THROUGH_OUTEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
												STEP_THROUGH_INEDGES(w1, e4, w2){
													if(TT3 > 0){break;}
													if(layer_mem[w2 -1] == layer_mem[w1 - 1]){
														TT3++;
													}
												}
											}
										}
									}
									if(tout_used[u - 1] == 0){
										change += ((isedge ? - (TT2 == 0) : (TT2 == 0)));
									}
									if(hout_used[v - 1] == 0){
										change += ((isedge ? - (TT3 == 0) : (TT3 == 0)));
									}
									tout_used[u - 1] = 1;
									hout_used[v - 1] = 1;
									TT2 = 0;
									TT3 = 0;
								}
							}
						}
					}
				}
				for(j = 0; j < N_NODES; j++){
					tout_used[j] = 0;
					hout_used[j] = 0;
				}
			}
		}
		if(incident == 1){
			change += ((isedge ? - (TTbase > 0) : (TTbase > 0)));
		}else{
			change = (isedge ? - TTbase : TTbase);
		}
		TOGGLE_IF_MORE_TO_COME(i);
	}
	CHANGE_STAT[0] = change;
	UNDO_PREVIOUS_TOGGLES(i);
}

D_CHANGESTAT_FN(d_fourcycle_crosslayer){
	/* Declaring variables */
	int i, l1, l2, tl, hl, t_neighbour, h_neighbour, tnl, hnl, isedge, with_attr; 
	Vertex tail, head, layer_mem[N_NODES];
	Edge e1, e2;
	double attn, change, nodecov[N_NODES];
	
	/* Layer memberships */
	for(i = 0; i < N_NODES; i++){
		layer_mem[i] = INPUT_PARAM[2 + i];
	}
	l1 = INPUT_PARAM[0]; l2 = INPUT_PARAM[1];
	
	/* If there are nodecovs to consider */
	with_attr = N_INPUT_PARAMS > (N_NODES + 2);
	if(with_attr){
		for(i = 0; i < N_NODES; i++){
		nodecov[i] = INPUT_PARAM[2 + N_NODES + i];
		}
	}
	
	/* Start looping through the edges */
	CHANGE_STAT[0] = 0.0; change = 0.0;
	FOR_EACH_TOGGLE(i){
		tail = TAIL(i); head = HEAD(i);
		isedge = IS_UNDIRECTED_EDGE(tail, head);
		tl = layer_mem[tail - 1]; hl = layer_mem[head - 1];
		attn = 0.0;
		
		if(with_attr){
			attn = nodecov[tail - 1] + nodecov[head - 1];
		}
		
		/* If the edge is intralayer */
		if(tl == hl && ((tl == l1) || (tl == l2))){ /* All four combinations of out-out, out-in, in-out, and in-in. */
			STEP_THROUGH_OUTEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_OUTEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == hnl && tnl != tl && ((tnl == l1) || (tnl == l2)) && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
			STEP_THROUGH_OUTEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_INEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == hnl && tnl != tl && ((tnl == l1) || (tnl == l2)) && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
			STEP_THROUGH_INEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_OUTEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == hnl && tnl != tl && ((tnl == l1) || (tnl == l2)) && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
			STEP_THROUGH_INEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_INEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == hnl && tnl != tl && ((tnl == l1) || (tnl == l2)) && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
		}
		/* If the edge is interlayer */
		if(tl != hl && ((tl == l1) || (tl == l2)) && ((hl == l1) || (hl == l2))){ /* All four combinations of out-out, out-in, in-out, and in-in. */
			STEP_THROUGH_OUTEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_OUTEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == tl && hnl == hl && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
			STEP_THROUGH_OUTEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_INEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == tl && hnl == hl && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
			STEP_THROUGH_INEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_OUTEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == tl && hnl == hl && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){ 
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
			STEP_THROUGH_INEDGES(tail, e1, t_neighbour){
				STEP_THROUGH_INEDGES(head, e2, h_neighbour){
					tnl = layer_mem[t_neighbour - 1]; hnl = layer_mem[h_neighbour - 1];
					if(tnl == tl && hnl == hl && IS_UNDIRECTED_EDGE(t_neighbour, h_neighbour)){ /* Conditions for a crosslayer four cycle */
						if(with_attr){
							change += (attn + nodecov[t_neighbour - 1] + nodecov[h_neighbour - 1]) * (isedge ? - 1 : 1);
						} else {
							change += (isedge ? - 1 : 1);
						}
					}
				}
			}
		}
		TOGGLE_IF_MORE_TO_COME(i);
	}	
	CHANGE_STAT[0] = change;
	UNDO_PREVIOUS_TOGGLES(i);
}