/*****
** References
** - J. MacQueen, "Some methods for classification and analysis
**   of multivariate observations", Fifth Berkeley Symposium on
**   Math Statistics and Probability, 281-297, 1967.
** - I.S. Dhillon and D.S. Modha, "A data-clustering algorithm
**   on distributed memory multiprocessors",
**   Large-Scale Parallel Data Mining, 245-260, 1999.
** Yuanming Chen, 2008-05-08
*/

#ifndef BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP
#define BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP

#include <cmath>
#include <float.h>
//#include "common.hpp"
#include <vector>
#include <list>
#include <cassert>

namespace boost {
  namespace algorithm {
      namespace cluster {
          namespace detail {  
                template<typename AttributeType, typename differenceType>
                //The original C function
                int *k_means(AttributeType **data, int n, int m, int k, differenceType eps, AttributeType **centroids)
                {
                   /* output cluster label for each data point */
                   int *labels = (int*)calloc(n, sizeof(int));

                   int h, i, j; /* loop counters, of course :) */
                   int *counts = (int*)calloc(k, sizeof(int)); /* size of each cluster */
                   AttributeType old_error, error = FLT_MAX; /* sum of squared euclidean distance */
                   AttributeType **c = centroids ? centroids : (AttributeType**)calloc(k, sizeof(AttributeType*));
                   AttributeType **c1 = (AttributeType**)calloc(k, sizeof(AttributeType*)); /* temp centroids */

                   //assert(data && k > 0 && k <= n && m > 0 && t >= 0); /* for debugging */

                   /****
                   ** initialization */

                   for (h = i = 0; i < k; h += n / k, i++) {
                      c1[i] = (AttributeType*)calloc(m, sizeof(AttributeType));
                      if (!centroids) {
                         c[i] = (AttributeType*)calloc(m, sizeof(AttributeType));
                      }
                      /* pick k points as initial centroids */
                      for (j = m; j-- > 0; c[i][j] = data[h][j]);
                   }

                   /****
                   ** main loop */

                   do {
                      /* save error from last step */
                      old_error = error, error = 0;

                      /* clear old counts and temp centroids */
                      for (i = 0; i < k; counts[i++] = 0) {
                         for (j = 0; j < m; c1[i][j++] = 0);
                      }

                      for (h = 0; h < n; h++) {
                         /* identify the closest cluster */
                         AttributeType min_distance = FLT_MAX;
                         for (i = 0; i < k; i++) {
                            AttributeType distance = 0;
                            for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
                            if (distance < min_distance) {
                               labels[h] = i;
                               min_distance = distance;
                            }
                         }
                         /* update size and temp centroid of the destination cluster */
                         for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
                         counts[labels[h]]++;
                         /* update standard error */
                         error += min_distance;
                      }

                      for (i = 0; i < k; i++) { /* update all centroids */
                         for (j = 0; j < m; j++) {
                            c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
                         }
                      }

                   } while (fabs(error - old_error) > eps);

                   /****
                   ** housekeeping */

                   for (i = 0; i < k; i++) {
                      if (!centroids) {
                         free(c[i]);
                      }
                      free(c1[i]);
                   }

                   if (!centroids) {
                      free(c);
                   }
                   free(c1);

                   free(counts);

                   return labels;
                }
          } //End of details namespace

            template<typename PointType>
            struct KMeansCluster {
                PointType centroid;
                std::vector<int> points; //The indice of points are stored here 
            };

            template <typename KMeansCluster> 
            struct KMeansClustering { 
                typedef std::vector< KMeansCluster > type; 
                type clusters;
            };

            /** 
            * @param first: the first data point's iterator
            * @param last: the last data point's iterator
            * @param k: the k value for the k-mean algorithm
            * @return collections of clusters
            */
            template <typename NTupleIter>
            typename KMeansClustering< typename KMeansCluster<typename NTupleIter::value_type> >
            k_means(NTupleIter first, NTupleIter last, unsigned k, 
                   typename NTupleIter::difference_type const & eps)
            {
                typedef NTupleIter::difference_type DistanceType;
                typedef NTupleIter::value_type PointType;
                typedef PointType::value_type AttributeType; //For the c funtion test, it will be a double type
                const DistanceType knumOfPoints = last - first; //The n variable in the C function
                const size_t knDimension = PointType::size(); //The m variable in the C function

                AttributeType** ppData = new AttributeType* [knumOfPoints];
                AttributeType** centroids = new AttributeType* [k]; 
                //Pre-allocate the result array
                for(size_t nCentroid = 0; nCentroid < k; nCentroid++)
                {
                    centroids[nCentroid] = new AttributeType[knDimension];
                }

                int nIndex = 0;
                for(NTupleIter iter = first; iter != last; iter++, nIndex++)
                {
                    PointType& pt= *iter; //A point
                    ppData[nIndex] = new AttributeType[knDimension];
                    for(unsigned int nAttribute = 0; nAttribute < knDimension; nAttribute++)
                    {
                        ppData[nIndex][nAttribute] = pt[nAttribute];
                    }
                }

                int* labels = detail::k_means(ppData, (int) knumOfPoints, (int) knDimension, k, eps, centroids);

                typedef KMeansCluster<PointType> KMeansClusterType;
                KMeansClustering< KMeansClusterType > clustering;
                for(size_t nCentroid = 0; nCentroid < k; nCentroid++)
                {
                    
                    KMeansClusterType cluster;
                    PointType centroid;
                    for(unsigned int nAttribute = 0; nAttribute < knDimension; nAttribute++)
                    {
                        centroid[nAttribute] = centroids[nCentroid][nAttribute];
                    }
                    cluster.centroid = centroid;
                    clustering.clusters.push_back(cluster);
                    delete[] centroids[nCentroid];
                }

                for(int nPoint = 0; nPoint < knumOfPoints; nPoint++)
                {
                    int nCentroidIndex = labels[nPoint];
                    clustering.clusters[nCentroidIndex].points.push_back(nPoint);
                    delete[] ppData[nPoint];
                }

                delete[] centroids;
                delete[] ppData;
                delete[] labels;

                return clustering;
            }
        } //End of cluster namespace
    } //End of algorithm namespace
} //End of boost namespace

#endif // BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP
