<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.11.1" name="Enemys" tilewidth="32" tileheight="64" tilecount="2" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <transformations hflip="1" vflip="1" rotate="1" preferuntransformed="0"/>
 <tile id="0" x="0" y="128" width="32" height="64">
  <properties>
   <property name="EnemyType" value="fire"/>
  </properties>
  <image source="../../../Images/Enemies/Fire.png" width="128" height="192"/>
 </tile>
 <tile id="1">
  <properties>
   <property name="EnemyType" value="spike"/>
  </properties>
  <image source="../../../Images/Enemies/Spikes.png" width="32" height="32"/>
 </tile>
</tileset>
