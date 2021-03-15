#ifndef LINE3D_H
#define LINE3D_H
class QWT3D_EXPORT Line3D: public VertexEnrichment
 {
  public:
  Line3D();
  Line3D(double thick,bool smooth);
 Qwt3D::Enrichment * clone() const{ return new Line3D(*this);}

 void configure(double thick, bool smooth);
 void drawBegin();
 void drawEnd();
 virtual void draw(Qwt3D::Triple const&);

 virtual void draw();

 virtual void add(Qwt3D::Triple const & t);
 virtual void setLineColor(RGBA color);

 private:
 bool smooth_;
 double lineThick;
 GLboolean oldstate_;

 std::vector<Qwt3D::Triple> lineData;

 RGBA rgba;
 };

#endif // LINE3D_H
