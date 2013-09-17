#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


typedef int distance_type;

template< class m_object_type >             // metric object type
class VPTree
{
    typedef unsigned int uint;
    typedef distance_type (*distance_function)(const m_object_type&,const m_object_type&);

public:

    VPTree( distance_function _distance, vector< m_object_type > objects ):
        distance(_distance)
    {
        root = create_tree( objects );
    }

    vector< pair< m_object_type, distance_type > > query( m_object_type &q, distance_type r )
    {
        vector< pair< m_object_type, distance_type > > ret;
        obtain_candidates( q, r, root, ret );
        return ret;
    }

private:

    struct node
    {
        m_object_type obj;
        node* children[2];
        distance_type m;

        node( m_object_type _obj ): obj(_obj)
        {
            // Ctor
        }
    };



    distance_function distance;
    node* root;

    node* create_tree( vector< m_object_type > objects )
    {

        if( objects.empty() )
            return NULL;

        m_object_type vantage_point = objects.back();
        objects.pop_back();

        distance_type distances[ objects.size() ];

        for( uint i=0; i < objects.size(); ++i )
        {
            distances[i] = (*distance)( vantage_point, objects[i] );
        }

        distance_type m = distances[ objects.size()/2 ];
        vector< m_object_type > left_branch;
        vector< m_object_type > right_branch;

        for( uint i=0; i < objects.size(); ++i )
        {
            if( distances[i] <= m )
                left_branch.push_back( objects[i] );
            else
                right_branch.push_back( objects[i] );
        }

        node *ret = new node( vantage_point );
        ret->children[0] = create_tree( left_branch );
        ret->children[1] = create_tree( right_branch );
        ret->m = m;

        return ret;
    }

    void obtain_candidates( m_object_type& q, distance_type& r, node* subtree, vector< pair< m_object_type, distance_type > > &candidates )
    {
        if( subtree )
        {
            distance_type d = distance( subtree->obj, q );
            if( d <= r )
                candidates.push_back( pair< m_object_type, distance_type >( subtree->obj, d ) );

            if( d - r <= subtree->m )
                obtain_candidates( q, r, subtree->children[0], candidates );

            if( d + r > subtree->m )
                obtain_candidates( q, r, subtree->children[1], candidates );
        }

    }

};












#include <windows.h>
#include "l_distance.h"

#define TOLERANCIA 2
#define MAXPAL 10


template< class T >
struct sort_candidates{
    bool operator()( const pair<T, distance_type> &p1, const pair<T, distance_type> &p2){
        return p1.second < p2.second;
    }
};


int main()
{
    typedef unsigned int uint;

    vector<string> m_espace;

    // Llenamos m_espace con el archivo "dict.txt"

    ifstream dict("dict.txt");
    string buffer;
    while( !dict.eof() )
    {
        dict >> buffer;
        m_espace.push_back(buffer);
    }
    dict.close();

    // Creamos la estructura

    VPTree<string> vpt(&l_distance, m_espace);

    // Creamos la interfaz

    while(1)
    {
        string q;
        cout << "Ingrese una palabra erronea: ";
        cin >> q;

        vector< pair< string, distance_type > > ret = vpt.query( q, TOLERANCIA );

        if( !ret.empty() )
        {
            sort( ret.begin(), ret.end(), sort_candidates<string>() );

            if( ret[0].second == 0 )
            {
                cout << "La palabra es correcta!" << endl;
            }
            else
            {
                cout << "Quiza quizo decir: " << endl;
                for( uint i = 0; i < ret.size() && i < MAXPAL; ++i )
                {
                    cout << "Distancia: " << ret[i].second << "\tPalabra: " << ret[i].first << endl;
                }
            }
        }
        else
        {
            cout << "No hay sugerencias" << endl;
        }


        system("pause");
        system("cls");
    }



    return 0;
}
