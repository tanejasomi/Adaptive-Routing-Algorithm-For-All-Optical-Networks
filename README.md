# Adaptive Routing Algorithm For All Optical Networks
Wavelength-division multiplexing (WDM) offers the capability to handle the increasing demand of network traffic in a manner that takes advantage of already deployed optical fibers. Light paths are optical connections carried end-to-end over a wavelength on each intermediate link. Wavelengths are the main resource in WDM networks. Blocking and resource utilization are the key performance parameters in the design of an all-optical network. Due to the inherent channel constraints, a dynamic control mechanism is required to efficiently utilize the resource to maximize light path connections.

In this project, we have developed and implemented DWR (Dynamic Wavelength Routing) Algorithms. 

The main objective of our proposed algorithm is to minimize the combined cost of primary and secondary paths. The proposed model includes various factors such as mean load variance of the load on the route, hop length and number of converters while selecting the appropriate routes. 

To evaluate the performance of the proposed algorithm, we have developed a simulator based on C language. The performance studies are conducted for two different networks, INDIANET and NSFNET. We have computed the blocking probability, average hop-length, mean link utilization and its standard deviation. 

It is observed that the proposed scheme leads to improvement in blocking performance and link utilization in comparison with the existing algorithms such as Least Loaded Routing (LLR) and Weighted Least-Congestion Routing (WLCR).
