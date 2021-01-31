# Dissertation
Electric vehicle routing problem source code.

"The proposed EVRP model formulation follows a classic VRP formulation: let G = (NF, A) be a graph where
vertex set N is a combination of the customer set N0 = {1,2,...,i,...,j,…,n} and the depot {O}; F={n+1,n+2,...,n+s,
s 0s0} is a set of charging stations. The set of charging stations F includes a charging station O1 located at the
depot. The set A = {(i,j), i,j N, i j} corresponds to all the possible arcs connecting vertices of N. Note the
differences in graph representation between a traditional VRP and the EVRP. In the traditional VRP, the vertices are
all serviced customer points plus the depot and each pair of vertices is connected exactly once (one and only one arc),
i.e. a complete graph. In the EVRP, the vertices also include all the charging stations in the study area, some of
which may not be visited at all and the others may be visited multiple times in a given strategy. Each arc (i,j) is
associated with a non-negative travel time tij and distance dij. Travel speeds vij are assumed to be constant over an
arc. There are at most M number of EVs that can be dispatched to perform the delivery/pickup tasks. Fig. 1
graphically describes the EVRP.
The battery re-charging rule is defined as follows: when an EV starts the route (daily operation) at the depot (O),
its battery is fully charged at O1; the EV can be re-charged once or more at any of the charging stations in F during
routing; and when it returns to the depot after accomplishing all the tasks, it is recharged to the full battery capacity
at O1 at the end of the daily operation.
The proposed EVRP must satisfy the following additional conditions or assumptions.
1. There is one single depot at which all vehicle routes begin and end.
2. Travel speed on each arc is constant and may vary across arcs;
3. Battery re-charging rate (in joules/hr) is constant;
4. The battery is re-charged to full each time after visiting a charging station;
5. The total work hour limit of an EV is 8 hours;
6. There is no idling time on an arc or at stops (either customer or charging station);
7. No time window constraint is considered for the customers.
8. There is a mix of delivery and pickup service on a vehicle route." - [1] (Jane Lin, Wei Zhou, Ouri Wolfson)



[1]
Jane Lin, Wei Zhou, Ouri Wolfson,
Electric Vehicle Routing Problem,
Transportation Research Procedia,
Volume 12,
2016,
Pages 508-521,
ISSN 2352-1465,
https://doi.org/10.1016/j.trpro.2016.02.007.
(http://www.sciencedirect.com/science/article/pii/S2352146516000089)
Abstract: This paper presents a general Electric Vehicle Routing Problem (EVRP) that finds the optimal routing strategy with minimal travel time cost and energy cost as well as number of EVs dispatched. This is the first EVRP model to consider the vehicle load effect on battery consumption. As demonstrated with a case study in Austin TX, the effect of vehicle load on routing strategy cannot be ignored. Compared to diesel truck VRP, EVRP has comparable travel time and distance but long en-route re-charging time, which translates into a considerable amount of additional labor cost. Lastly, the network topology greatly affects the routing strategies.
Keywords: electric vehicle routing problem (EVRP); charging station location; battery capacity; charging technology; graph representation
