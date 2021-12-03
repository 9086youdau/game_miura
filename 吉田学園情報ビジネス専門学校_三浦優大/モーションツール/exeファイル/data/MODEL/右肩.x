xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 63;
 4.05121;-2.30918;-0.00000;,
 2.03475;-2.60170;-3.17860;,
 4.05121;-2.30918;-3.92896;,
 4.05121;-2.30918;-0.00000;,
 -0.18065;-3.77244;-1.21412;,
 4.05121;-2.30918;-0.00000;,
 -0.18065;-3.77244;1.21412;,
 4.05121;-2.30918;-0.00000;,
 2.03475;-2.60170;3.17860;,
 4.05121;-2.30918;-0.00000;,
 4.05121;-2.30918;3.92896;,
 4.05121;-2.30918;-0.00000;,
 6.43514;-0.67164;3.17860;,
 4.05121;-2.30918;-0.00000;,
 7.78639;0.34740;1.21412;,
 4.05121;-2.30918;-0.00000;,
 7.78639;0.34740;-1.21412;,
 4.05121;-2.30918;-0.00000;,
 6.43514;-0.67164;-3.17860;,
 4.05121;-2.30918;-0.00000;,
 4.05121;-2.30918;-3.92896;,
 -12.31986;0.24568;-3.29963;,
 -2.36969;0.24568;-3.92896;,
 -2.38361;-1.83402;-3.17860;,
 -12.31986;-1.50089;-2.66946;,
 -2.39222;-3.11939;-1.21412;,
 -12.31986;-2.58038;-1.01964;,
 -2.39222;-3.11939;1.21412;,
 -12.31986;-2.58038;1.01964;,
 -2.38361;-1.83402;3.17860;,
 -12.31986;-1.50089;2.66946;,
 -2.36969;0.24568;3.92896;,
 -12.31986;0.24568;3.29963;,
 -12.31986;0.24568;3.29963;,
 -2.35579;2.32543;3.17860;,
 -12.31986;1.99230;2.66946;,
 -2.34719;3.61078;1.21412;,
 -12.31986;3.07177;1.01964;,
 -2.34719;3.61078;1.21412;,
 -2.34719;3.61078;-1.21412;,
 -12.31986;3.07177;-1.01964;,
 -2.34719;3.61078;-1.21412;,
 -2.35579;2.32543;-3.17860;,
 -12.31986;1.99230;-2.66946;,
 -2.36969;0.24568;-3.92896;,
 -12.31986;0.24568;-3.29963;,
 0.62826;-0.40283;-4.08890;,
 -0.05099;-1.87014;-3.30799;,
 -0.69237;-3.25562;-1.26354;,
 -0.69237;-3.25562;-1.26354;,
 -0.69237;-3.25562;1.26354;,
 -0.69237;-3.25562;1.26354;,
 -0.05099;-1.87014;3.30799;,
 0.62826;-0.40283;4.08890;,
 1.55471;1.59846;3.30799;,
 1.55471;1.59846;3.30799;,
 2.12146;2.82274;1.26354;,
 2.12146;2.82274;1.26354;,
 2.12146;2.82274;-1.26354;,
 2.12146;2.82274;-1.26354;,
 1.55471;1.59846;-3.30799;,
 1.55471;1.59846;-3.30799;,
 0.62826;-0.40283;-4.08890;;
 
 40;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,16;,
 3;19,20,18;,
 4;21,22,23,24;,
 4;24,23,25,26;,
 4;26,25,27,28;,
 4;28,27,29,30;,
 4;30,29,31,32;,
 4;33,31,34,35;,
 4;35,34,36,37;,
 4;37,38,39,40;,
 4;40,41,42,43;,
 4;43,42,44,45;,
 4;46,2,1,47;,
 4;47,23,22,46;,
 4;47,1,4,48;,
 4;48,25,23,47;,
 4;49,4,6,50;,
 4;50,27,25,49;,
 4;51,6,8,52;,
 4;52,29,27,51;,
 4;52,8,10,53;,
 4;53,31,29,52;,
 4;53,10,12,54;,
 4;54,34,31,53;,
 4;55,12,14,56;,
 4;56,36,34,55;,
 4;57,14,16,58;,
 4;58,39,38,57;,
 4;59,16,18,60;,
 4;60,42,41,59;,
 4;61,18,20,62;,
 4;62,44,42,61;;
 
 MeshMaterialList {
  141;
  40;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  0,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.680800;0.520800;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.650000;0.553150;0.423150;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.147200;0.147200;0.147200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.090000;0.047000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;0.847000;0.651000;1.000000;;
   3.000000;
   0.200000;0.200000;0.200000;;
   0.200000;0.169400;0.130200;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.145000;0.192000;0.192000;1.000000;;
   29.000000;
   0.400000;0.400000;0.400000;;
   0.014500;0.019200;0.019200;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.408000;0.376000;0.424000;1.000000;;
   30.000000;
   0.600000;0.600000;0.600000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.961000;0.949000;1.000000;1.000000;;
   60.000000;
   0.700000;0.700000;0.700000;;
   0.480500;0.474500;0.500000;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.830000;0.234060;0.273070;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.650000;0.183300;0.213850;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.650000;0.650000;0.650000;;
  }
  Material {
   0.420000;0.000000;0.009600;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.341250;0.000000;0.007800;;
  }
  Material {
   0.091200;0.065600;0.044000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.075240;0.054120;0.036300;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "�s��.bmp";
   }
  }
  Material {
   0.800000;0.674400;0.000000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.843000;0.000000;;
  }
  Material {
   0.517600;0.517600;0.517600;0.610000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.304090;0.304090;0.304090;;
  }
  Material {
   0.426400;0.367200;0.367200;0.610000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250510;0.215730;0.215730;;
  }
  Material {
   0.800000;0.288800;0.000000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.610000;0.220210;0.000000;;
  }
  Material {
   0.455200;0.138400;0.000000;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.347090;0.105530;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.398400;0.398400;0.398400;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.800000;0.470000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.670000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.540000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.176000;0.279200;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.671200;0.000000;0.194400;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.382400;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.500000;0.323500;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;0.580000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.647000;0.000000;;
  }
  Material {
   0.800000;0.150400;0.300800;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.150400;0.300800;0.430000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.490000;0.092120;0.184240;;
  }
  Material {
   0.800000;0.150400;0.300800;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.188000;0.376000;;
  }
  Material {
   0.800000;0.464000;0.439200;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.464000;0.439200;0.450000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.520000;0.301600;0.285480;;
  }
  Material {
   0.800000;0.464000;0.439200;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.580000;0.549000;;
  }
  Material {
   0.668000;0.207200;0.216800;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.649600;0.200800;0.207200;0.480000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.414120;0.128010;0.132090;;
  }
  Material {
   0.702400;0.216800;0.225600;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.878000;0.271000;0.282000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.540000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.550000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.000000;0.000000;;
  }
  Material {
   0.640000;0.527200;0.721600;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.380000;0.125600;0.404800;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.586400;0.499200;0.721600;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.359170;0.305760;0.441980;;
  }
  Material {
   0.000000;0.000000;0.800000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.037600;0.288800;0.721600;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.439200;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.329600;0.542400;0.800000;0.540000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.455200;0.527200;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.455200;0.527200;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.335710;0.388810;;
  }
  Material {
   0.000000;0.455200;0.527200;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.569000;0.659000;;
  }
  Material {
   0.210400;0.191200;0.520800;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.210400;0.191200;0.520800;0.550000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.134130;0.121890;0.332010;;
  }
  Material {
   0.210400;0.191200;0.520800;0.540000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.263000;0.239000;0.651000;;
  }
  Material {
   0.313600;0.473600;0.524000;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.310400;0.467200;0.517600;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.182360;0.274480;0.304090;;
  }
  Material {
   0.351200;0.530400;0.586400;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.263400;0.397800;0.439800;;
  }
  Material {
   0.000000;0.737600;0.700000;0.580000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.553200;0.484800;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.276600;0.242400;;
  }
  Material {
   0.000000;0.800000;0.790400;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;1.000000;0.988000;;
  }
  Material {
   0.012800;0.100000;0.401600;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.153600;0.702400;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.051840;0.237060;;
  }
  Material {
   0.040800;0.000000;0.800000;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.051000;0.000000;1.000000;;
  }
  Material {
   0.000000;0.759200;0.520800;0.470000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.420000;0.448800;0.341600;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.103200;0.445600;0.103200;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.796800;0.000000;0.060000;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.996000;0.000000;0.075000;;
  }
  Material {
   0.490000;0.334180;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.540000;0.368280;0.000000;;
  }
  Material {
   0.445600;0.448800;0.000000;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.140800;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.000000;0.176000;;
  }
  Material {
   0.800000;0.000000;0.524000;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.580000;0.000000;0.379900;;
  }
  Material {
   1.000000;0.847000;0.651000;1.000000;;
   3.000000;
   0.200000;0.200000;0.200000;;
   0.200000;0.169400;0.130200;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.145000;0.192000;0.192000;1.000000;;
   29.000000;
   0.400000;0.400000;0.400000;;
   0.014500;0.019200;0.019200;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.408000;0.376000;0.424000;1.000000;;
   30.000000;
   0.600000;0.600000;0.600000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.961000;0.949000;1.000000;1.000000;;
   60.000000;
   0.700000;0.700000;0.700000;;
   0.480500;0.474500;0.500000;;
   TextureFilename {
    "miku_tex.jpg";
   }
  }
  Material {
   0.165000;0.165000;0.165000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.728000;0.618400;0.473600;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.591500;0.502450;0.384800;;
  }
  Material {
   0.830000;0.234060;0.273070;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.650000;0.183300;0.213850;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.650000;0.650000;0.650000;;
  }
  Material {
   0.420000;0.000000;0.009600;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.341250;0.000000;0.007800;;
  }
  Material {
   0.119200;0.119200;0.119200;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.096850;0.096850;0.096850;;
  }
  Material {
   0.091200;0.065600;0.044000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.075240;0.054120;0.036300;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "�s��.bmp";
   }
  }
  Material {
   0.800000;0.674400;0.000000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.843000;0.000000;;
  }
  Material {
   0.517600;0.517600;0.517600;0.610000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.304090;0.304090;0.304090;;
  }
  Material {
   0.426400;0.367200;0.367200;0.610000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250510;0.215730;0.215730;;
  }
  Material {
   0.800000;0.288800;0.000000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.610000;0.220210;0.000000;;
  }
  Material {
   0.455200;0.138400;0.000000;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.347090;0.105530;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.398400;0.398400;0.398400;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.800000;0.470000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.670000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.540000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.176000;0.279200;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.671200;0.000000;0.194400;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.382400;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.500000;0.323500;0.000000;;
  }
  Material {
   0.800000;0.517600;0.000000;0.580000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.647000;0.000000;;
  }
  Material {
   0.800000;0.150400;0.300800;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.150400;0.300800;0.430000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.490000;0.092120;0.184240;;
  }
  Material {
   0.800000;0.150400;0.300800;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.188000;0.376000;;
  }
  Material {
   0.800000;0.464000;0.439200;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.464000;0.439200;0.450000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.520000;0.301600;0.285480;;
  }
  Material {
   0.800000;0.464000;0.439200;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.580000;0.549000;;
  }
  Material {
   0.668000;0.207200;0.216800;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.649600;0.200800;0.207200;0.480000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.414120;0.128010;0.132090;;
  }
  Material {
   0.702400;0.216800;0.225600;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.878000;0.271000;0.282000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.540000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;0.550000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.000000;0.000000;;
  }
  Material {
   0.640000;0.527200;0.721600;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.380000;0.125600;0.404800;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.586400;0.499200;0.721600;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.359170;0.305760;0.441980;;
  }
  Material {
   0.000000;0.000000;0.800000;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.037600;0.288800;0.721600;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.439200;0.510000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.329600;0.542400;0.800000;0.540000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.455200;0.527200;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.455200;0.527200;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.335710;0.388810;;
  }
  Material {
   0.000000;0.455200;0.527200;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.569000;0.659000;;
  }
  Material {
   0.210400;0.191200;0.520800;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.210400;0.191200;0.520800;0.550000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.134130;0.121890;0.332010;;
  }
  Material {
   0.210400;0.191200;0.520800;0.540000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.263000;0.239000;0.651000;;
  }
  Material {
   0.313600;0.473600;0.524000;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.310400;0.467200;0.517600;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.182360;0.274480;0.304090;;
  }
  Material {
   0.351200;0.530400;0.586400;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.263400;0.397800;0.439800;;
  }
  Material {
   0.000000;0.737600;0.700000;0.580000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.553200;0.484800;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.276600;0.242400;;
  }
  Material {
   0.000000;0.800000;0.790400;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;1.000000;0.988000;;
  }
  Material {
   0.012800;0.100000;0.401600;0.520000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.153600;0.702400;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.051840;0.237060;;
  }
  Material {
   0.040800;0.000000;0.800000;0.490000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.051000;0.000000;1.000000;;
  }
  Material {
   0.000000;0.759200;0.520800;0.470000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.420000;0.448800;0.341600;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.103200;0.445600;0.103200;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.796800;0.000000;0.060000;0.560000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.996000;0.000000;0.075000;;
  }
  Material {
   0.490000;0.334180;0.000000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.540000;0.368280;0.000000;;
  }
  Material {
   0.445600;0.448800;0.000000;0.460000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.140800;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;0.000000;0.176000;;
  }
  Material {
   0.800000;0.000000;0.524000;0.530000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.580000;0.000000;0.379900;;
  }
 }
 MeshNormals {
  48;
  0.429639;-0.903001;-0.000000;,
  0.364384;-0.931249;-0.000000;,
  0.043126;-0.956736;-0.287744;,
  -0.122394;-0.965165;-0.231250;,
  0.112197;-0.950186;0.290791;,
  -0.014825;-0.879982;0.474775;,
  0.575008;-0.818089;-0.009793;,
  0.581674;-0.813363;-0.009793;,
  0.581674;-0.813363;0.009793;,
  0.575008;-0.818089;0.009793;,
  -0.050067;-0.008314;-0.998711;,
  -0.072605;-0.624154;-0.777920;,
  -0.066237;-0.955190;-0.288485;,
  -0.066237;-0.955190;0.288485;,
  -0.072605;-0.624154;0.777920;,
  -0.050067;-0.008314;0.998711;,
  0.013849;0.621409;0.783364;,
  0.053051;0.957068;0.284967;,
  0.053051;0.957068;-0.284967;,
  0.013849;0.621409;-0.783364;,
  -0.063126;0.000184;-0.998006;,
  -0.060869;-0.618833;-0.783160;,
  -0.057519;-0.956733;-0.285226;,
  -0.057519;-0.956733;0.285226;,
  -0.060869;-0.618833;0.783160;,
  -0.063126;0.000184;0.998006;,
  -0.060717;0.619067;0.782987;,
  -0.057285;0.956777;0.285126;,
  -0.057285;0.956777;-0.285126;,
  -0.060717;0.619067;-0.782987;,
  -0.018655;-0.065097;-0.997705;,
  -0.185638;-0.607112;-0.772628;,
  -0.341773;-0.880686;-0.327998;,
  -0.341773;-0.880686;0.327998;,
  -0.185638;-0.607112;0.772628;,
  -0.018655;-0.065097;0.997705;,
  0.180112;0.576701;0.796854;,
  0.278289;0.914393;0.294009;,
  0.278289;0.914393;-0.294009;,
  0.180112;0.576701;-0.796854;,
  -0.000130;-0.113859;-0.993497;,
  -0.167743;-0.466770;-0.868325;,
  -0.710602;-0.703594;0.000000;,
  -0.000130;-0.113859;0.993497;,
  0.269931;0.526987;0.805867;,
  0.390670;0.869428;0.302444;,
  0.390670;0.869428;-0.302444;,
  0.269931;0.526987;-0.805867;;
  40;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,1,5;,
  3;0,6,1;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,9,8;,
  3;0,1,9;,
  4;20,10,11,21;,
  4;21,11,12,22;,
  4;22,12,13,23;,
  4;23,13,14,24;,
  4;24,14,15,25;,
  4;25,15,16,26;,
  4;26,16,17,27;,
  4;27,17,18,28;,
  4;28,18,19,29;,
  4;29,19,10,20;,
  4;30,40,41,31;,
  4;31,11,10,30;,
  4;31,2,3,32;,
  4;32,12,11,31;,
  4;32,3,42,33;,
  4;33,13,12,32;,
  4;33,4,5,34;,
  4;34,14,13,33;,
  4;34,5,43,35;,
  4;35,15,14,34;,
  4;35,43,44,36;,
  4;36,16,15,35;,
  4;36,44,45,37;,
  4;37,17,16,36;,
  4;37,45,46,38;,
  4;38,18,17,37;,
  4;38,46,47,39;,
  4;39,19,18,38;,
  4;39,47,40,30;,
  4;30,10,19,39;;
 }
 MeshTextureCoords {
  63;
  0.050000;0.000000;,
  0.100000;0.000000;,
  0.000000;0.000000;,
  0.150000;0.000000;,
  0.200000;0.000000;,
  0.250000;0.000000;,
  0.300000;0.000000;,
  0.350000;0.000000;,
  0.400000;0.000000;,
  0.450000;0.000000;,
  0.500000;0.000000;,
  0.550000;0.000000;,
  0.600000;0.000000;,
  0.650000;0.000000;,
  0.700000;0.000000;,
  0.750000;0.000000;,
  0.800000;0.000000;,
  0.850000;0.000000;,
  0.900000;0.000000;,
  0.950000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.495797;,
  0.000000;0.147852;,
  0.100000;0.148338;,
  0.100000;0.495797;,
  0.200000;0.148639;,
  0.200000;0.495797;,
  0.300000;0.148639;,
  0.300000;0.495797;,
  0.400000;0.148338;,
  0.400000;0.495797;,
  0.500000;0.147852;,
  0.500000;0.495797;,
  0.500000;0.495797;,
  0.600000;0.147365;,
  0.600000;0.495797;,
  0.700000;0.147065;,
  0.700000;0.495797;,
  0.700000;0.147064;,
  0.800000;0.147065;,
  0.800000;0.495797;,
  0.800000;0.147064;,
  0.900000;0.147365;,
  0.900000;0.495797;,
  1.000000;0.147852;,
  1.000000;0.495797;,
  0.000000;0.090447;,
  0.100000;0.086087;,
  0.200000;0.064861;,
  0.200000;0.064861;,
  0.300000;0.064861;,
  0.300000;0.064861;,
  0.400000;0.086087;,
  0.500000;0.090447;,
  0.600000;0.090885;,
  0.600000;0.090885;,
  0.700000;0.090389;,
  0.700000;0.090389;,
  0.800000;0.090389;,
  0.800000;0.090389;,
  0.900000;0.090885;,
  0.900000;0.090885;,
  1.000000;0.090447;;
 }
}
