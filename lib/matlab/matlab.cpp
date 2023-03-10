

#include "matlab.h"



matlab_matRead::matlab_matRead(std::string &file_name,
                               std::string &struct_name)
    :file_name_(file_name),struct_name_(struct_name){
    //1. open mat file
    file_array_=matOpen(file_name_.c_str(),"r");
    if (file_array_ == nullptr)
        std::cout<<"open mat file error!"<<std::endl;

    //2. open the working field in the mat file, and get the special struct
    struct_array_=matGetVariable(file_array_,struct_name_.c_str());
    if (struct_array_ == nullptr)
        std::cout<<"read struct error!"<<std::endl;
    struct_index=mxGetN(struct_array_);

    //3. read variable in the structure

    for (int i =0; i < (int) struct_index; i++)
    {
        prn_array_.push_back(mxGetField(struct_array_, i, "PRN"));
        prn_data_.push_back(*mxGetPr(prn_array_.at(i)));
        I_Ps_array_.push_back(mxGetField(struct_array_,i,"I_P"));
        Absolute_sample_array_.push_back(mxGetField(struct_array_,i,"absoluteSample"));
        CarrPhase_array_.push_back(mxGetField(struct_array_,i,"rawCarrPhase"));

        int n=mxGetN(I_Ps_array_.at(i));
        arma::mat I_Ps_tempMat(1,n);
        arma::mat sample_tempMat(1,n);
        arma::mat carrPhase_tempMat(1,n);
        for (int cnts = 0; cnts < n; ++cnts)
        {
            I_Ps_data_.push_back(*(mxGetPr(I_Ps_array_.at(i)) + cnts));
            Absolute_sample_data_.push_back(*(mxGetPr(Absolute_sample_array_.at(i))+cnts));
            CarrPhase_data_.push_back(*(mxGetPr(CarrPhase_array_.at(i))+cnts));
        }
        for (int j = 0; j < n; ++j)
        {
            I_Ps_tempMat(0,j)=I_Ps_data_.at(j);
            sample_tempMat(0,j)=Absolute_sample_data_.at(j);
            carrPhase_tempMat(0,j)=CarrPhase_data_.at(j);
        }
        I_Ps_mat_.push_back(I_Ps_tempMat);
        Absolute_sample_mat_.push_back(sample_tempMat);
        CarrPhase_mat_.push_back(carrPhase_tempMat);
        I_Ps_data_.clear();
        Absolute_sample_data_.clear();
        CarrPhase_data_.clear();
    }
    if (prn_array_.empty()||I_Ps_array_.empty())
        std::cout<<"read variable error!"<<std::endl;

    matClose(file_array_);
}

int matlab_matRead::calculate_xcor(const matlab_matRead& xcor_array, int target_prn) {
    int find_flag1= -1;
    int find_flag2= -1;
    for (int i = 0; i < I_Ps_mat_.size(); ++i) {
        if(prn_data_.at(i)==target_prn){
            find_flag1= i;
            break;
        }
    }
    for (int j = 0; j < xcor_array.getIPsMat().size(); ++j) {
        if(xcor_array.getPrnData().at(j) == target_prn){
            find_flag2= j;
            break;
        }
    }
    if (find_flag1==-1||find_flag2==-1)
    {
        std::cerr<<"cannot find target prn satellite";
        return 0;
    }
    arma::mat mat1=I_Ps_mat_.at(find_flag1).cols(0,999);
    arma::mat mat2=xcor_array.getIPsMat().at(find_flag2).cols(0,999);
    arma::mat index=arma::linspace(-999,999,mat1.size()+mat2.size()-1);
    index=index.t();
    arma::mat temp_mat=arma::join_horiz(arma::zeros(1,mat1.size()-1),mat2);
    temp_mat=arma::join_horiz(temp_mat,arma::zeros(1,mat1.size()-1));
    arma::mat xcor_mat(1,mat1.size()+mat2.size()-1);
    double xcor_value=0;
    for (int i = 0; i < mat1.size()+mat2.size()-1; ++i) {
        for (int j = 0; j < mat1.size(); ++j) {
            xcor_value+=mat1(0,j)*temp_mat(0,i+j);
        }
        xcor_mat(0,i)=xcor_value;
        xcor_value=0;
    }
    xcor_mat=arma::abs(xcor_mat);
    int xcor_index=xcor_mat.index_max();
    double xcor_max=xcor_mat.max();
    int mat2_move_index=(int)index(0,xcor_index);
    std::cout<<"xcor index is "<<xcor_index<<std::endl;
    std::cout<<"xcor max is "<<xcor_max<<std::endl;
    std::cout<<"mat2 need to move "<<mat2_move_index<<std::endl;


    //    //fft calculate
    //    arma::cx_mat fft_mat1=arma::fft(mat1.t());
    //    arma::cx_mat fft_mat2=arma::fft(mat2.t());
    //    //conj multi ifft abs
    //    fft_mat2=arma::conj(fft_mat2);
    //    arma::cx_mat xcor_mat=fft_mat1%fft_mat2/fft_mat1.size();
    //    xcor_mat=arma::ifft(xcor_mat);
    //    arma::mat xcor=arma::abs(xcor_mat);
    //    //find max correlate index
    //    int xcor_index=xcor.index_max();
    //    double xcor_value=xcor.max();
    //    std::cout<<"xcor index is "<<xcor_index<<std::endl;
    //    std::cout<<"xcor value is "<<xcor_value<<std::endl;

    return mat2_move_index;
}

double matlab_matRead::getAbsoluteSample(int syncIndex, int prn) {
    int target_prn_flag=-1;
    for (int i = 0; i < prn_data_.size(); ++i) {
        if (prn_data_.at(i)==prn)
        {
            target_prn_flag=i;
            break;
        }
    }
    if (target_prn_flag==-1){
        std::cerr<<"cannot find target prn!";
        return 0;
    }
    arma::mat target_mat=Absolute_sample_mat_.at(target_prn_flag);
    double target_data=target_mat(0,syncIndex);
    return target_data;
}

const std::vector<arma::mat> &matlab_matRead::getIPsMat() const {
    return I_Ps_mat_;
}

const std::vector<int> &matlab_matRead::getPrnData() const {
    return prn_data_;
}

arma::mat matlab_matRead::getCarrierPhase(int syncIndex, int prn,int sampleToProcess) {
    int target_prn_flag=-1;
    for (int i = 0; i < prn_data_.size(); ++i) {
        if (prn_data_.at(i)==prn)
        {
            target_prn_flag=i;
            break;
        }
    }
    if (target_prn_flag==-1){
        std::cerr<<"cannot find target prn!";
        return nullptr;
    }
    arma::mat target_mat=CarrPhase_mat_.at(target_prn_flag);
    target_mat=target_mat.cols(syncIndex,syncIndex+sampleToProcess-1);
    return target_mat;
}

const std::vector<arma::mat> &matlab_matRead::getCarrPhaseMat() const {
    return CarrPhase_mat_;
}


matlab_matRead::~matlab_matRead() = default;

