/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkLineSpatialObject_hxx
#define itkLineSpatialObject_hxx


#include "itkLineSpatialObject.h"

namespace itk
{
/** Constructor */
template< unsigned int TDimension >
LineSpatialObject< TDimension >
::LineSpatialObject()
{
  this->SetTypeName("LineSpatialObject");
  this->GetProperty().SetRed(1);
  this->GetProperty().SetGreen(0);
  this->GetProperty().SetBlue(0);
  this->GetProperty().SetAlpha(1);
}

/** Print the object. */
template< unsigned int TDimension >
void
LineSpatialObject< TDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  os << indent << "LineSpatialObject(" << this << ")" << std::endl;
  Superclass::PrintSelf(os, indent);
}

template< unsigned int TDimension >
bool
LineSpatialObject< TDimension >
::IsInsideInWorldSpace(const PointType & point, unsigned int depth,
  const std::string & name) const
{
  if( this->GetTypeName().find( name ) != std::string::npos )
    {
    auto it = this->m_Points.begin();
    auto itEnd = this->m_Points.end();

    PointType transformedPoint = this->GetObjectToWorldTransform()
      ->GetInverseTransform()->TransformPoint(point);

    if ( this->GetMyBoundingBoxInWorldSpace()->IsInside(transformedPoint) )
      {
      while ( it != itEnd )
        {
        bool match = true;
        for( unsigned int i=0; i<TDimension; ++i )
          {
          if ( ! Math::AlmostEquals( ( *it ).GetPositionInObjectSpace()[i],
                   transformedPoint[i] ) )
            {
            match = false;
            break;
            }
          }
        if( match )
          {
          return true;
          }
        it++;
        }
      }
    }

  if( depth > 0 )
    {
    return Superclass::IsInsideChildrenInWorldSpace( point, depth-1, name );
    }


  return false;
}

} // end namespace itk

#endif
