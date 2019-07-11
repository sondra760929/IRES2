class FrameContainer : public osg::Group
{
public:
	FrameContainer() : _frameSize(240.0f), _active(true) {}
	
	FrameContainer( const FrameContainer& copy, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY )
	:   osg::Group(copy, copyop),
		_frame(copy._frame), _frameSize(copy._frameSize), _active(copy._active)
	{}
	
	META_Node( osg, FrameContainer );
	
	void setFrame( osg::MatrixTransform* frame )
	{
		_frame = frame;
		if ( !containsNode(frame) ) addChild( frame );
	}
	
	osg::MatrixTransform* getFrame() { return _frame.get(); }
	const osg::MatrixTransform* getFrame() const { return _frame.get(); }
	
	void setFrameSize( float size ) { _frameSize = size; }
	float getFrameSize() const { return _frameSize; }
	
	void setActive( bool b ) { _active = b; }
	bool getActive() const { return _active; }
	
	void traverse( osg::NodeVisitor& nv )
	{
		if ( _frame.valid() )
		{
			if ( _active && nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
			{
				osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(&nv);
				
				float pixelSize = cv->pixelSize(_frame->getBound().center(), 0.48f);
				if ( pixelSize!=_frameSize )
				{
					float pixelScale = pixelSize>0.0f ? _frameSize/pixelSize : 1.0f;
					osg::Vec3d scaleFactor(pixelScale, pixelScale, pixelScale);
					
					osg::Vec3 trans = _frame->getMatrix().getTrans();
					_frame->setMatrix( osg::Matrix::scale(scaleFactor) * osg::Matrix::translate(trans) );
				}
			}
		}
		osg::Group::traverse(nv);
 	}
	
protected:
	osg::ref_ptr<osg::MatrixTransform> _frame;
	float _frameSize;
	bool _active;
};